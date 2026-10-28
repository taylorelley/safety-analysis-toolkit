/// Cancellation check shared with worker threads started on behalf of a request.
pub type Cancellation = std::sync::Arc<dyn Fn() -> bool + Send + Sync>;
thread_local! {static CANCEL:std::cell::RefCell<Option<Cancellation>>=const { std::cell::RefCell::new(None) };}
pub fn with_cancellation<T>(
    check: impl Fn() -> bool + Send + Sync + 'static,
    work: impl FnOnce() -> T,
) -> T {
    scoped_cancellation(Some(std::sync::Arc::new(check)), work)
}
/// The check installed on this thread, for propagation to worker threads.
pub fn current_cancellation() -> Option<Cancellation> {
    CANCEL.with(|c| c.borrow().clone())
}
pub fn scoped_cancellation<T>(check: Option<Cancellation>, work: impl FnOnce() -> T) -> T {
    let previous = CANCEL.with(|c| c.replace(check));
    // Restore the enclosing check, including on unwind.
    struct Reset(Option<Cancellation>);
    impl Drop for Reset {
        fn drop(&mut self) {
            CANCEL.with(|c| *c.borrow_mut() = self.0.take());
        }
    }
    let _reset = Reset(previous);
    work()
}
/// Applies `work` to each item on up to `jobs` threads that share this thread's
/// cancellation. Results keep item order; items skipped after cancellation are `None`.
pub fn parallel<I: Sync, T: Send>(
    items: &[I],
    jobs: usize,
    work: impl Fn(usize, &I) -> T + Sync,
) -> Vec<Option<T>> {
    use std::sync::{Mutex, atomic::AtomicUsize, atomic::Ordering};
    let next = AtomicUsize::new(0);
    let slots: Vec<Mutex<Option<T>>> = items.iter().map(|_| Mutex::new(None)).collect();
    let cancellation = current_cancellation();
    std::thread::scope(|scope| {
        for _ in 0..jobs.clamp(1, items.len().max(1)) {
            let (cancellation, next, slots, work) = (cancellation.clone(), &next, &slots, &work);
            scope.spawn(move || {
                scoped_cancellation(cancellation, || {
                    while !cancelled() {
                        let i = next.fetch_add(1, Ordering::Relaxed);
                        let Some(item) = items.get(i) else { break };
                        let result = work(i, item);
                        *slots[i].lock().unwrap_or_else(|e| e.into_inner()) = Some(result);
                    }
                })
            });
        }
    });
    slots
        .into_iter()
        .map(|s| s.into_inner().unwrap_or_else(|e| e.into_inner()))
        .collect()
}
pub static CLI_CANCELLED: std::sync::atomic::AtomicBool = std::sync::atomic::AtomicBool::new(false);
pub fn cancelled() -> bool {
    CLI_CANCELLED.load(std::sync::atomic::Ordering::Relaxed)
        || CANCEL.with(|c| c.borrow().as_ref().is_some_and(|check| check()))
}
use crate::model::Execution;
use anyhow::{Result, bail};
use std::{
    collections::BTreeMap,
    io::Read,
    path::Path,
    process::{Command, Stdio},
    time::{Duration, Instant},
};
const OUTPUT_LIMIT: usize = 4 * 1024 * 1024;
fn capture(mut reader: impl Read) -> (Vec<u8>, bool) {
    let mut out = Vec::new();
    let mut buf = [0; 8192];
    let mut truncated = false;
    loop {
        match reader.read(&mut buf) {
            Ok(0) => break,
            Ok(n) => {
                let keep = n.min(OUTPUT_LIMIT - out.len());
                out.extend_from_slice(&buf[..keep]);
                truncated |= keep < n
            }
            Err(_) => {
                truncated = true;
                break;
            }
        }
    }
    (out, truncated)
}
pub fn execute(
    exe: &Path,
    args: &[String],
    cwd: &Path,
    write_dir: &Path,
    seconds: u64,
    memory_mb: u64,
    require_filesystem_isolation: bool,
) -> Result<Execution> {
    if !exe.is_absolute() || !exe.is_file() {
        bail!("executable must be an existing absolute path")
    }
    anyhow::ensure!(!cancelled(), "request cancelled before execution");
    let executable_sha256 = crate::evidence::hash_file(exe)?;
    let env = BTreeMap::from([
        ("LC_ALL".to_string(), "C".to_string()),
        ("LANG".to_string(), "C".to_string()),
        ("HOME".to_string(), write_dir.display().to_string()),
        ("TMPDIR".to_string(), write_dir.display().to_string()),
    ]);
    let mut command = Command::new(exe);
    command
        .args(args)
        .current_dir(cwd)
        .env_clear()
        .envs(&env)
        .stdin(Stdio::null())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped());
    #[cfg(target_os = "linux")]
    {
        use std::os::unix::process::CommandExt;
        let output = std::ffi::CString::new(write_dir.as_os_str().as_encoded_bytes())?;
        unsafe {
            command.pre_exec(move || {
                if libc::setpgid(0, 0) != 0 {
                    return Err(std::io::Error::last_os_error());
                }
                for (resource, limit) in [
                    (libc::RLIMIT_AS, memory_mb * 1024 * 1024),
                    (libc::RLIMIT_CPU, seconds + 1),
                    (libc::RLIMIT_FSIZE, 64 * 1024 * 1024),
                    (libc::RLIMIT_NOFILE, 128),
                ] {
                    let lim = libc::rlimit {
                        rlim_cur: limit,
                        rlim_max: limit,
                    };
                    if libc::setrlimit(resource, &lim) != 0 {
                        return Err(std::io::Error::last_os_error());
                    }
                }
                isolate(&output, require_filesystem_isolation)
            })
        };
    }
    #[cfg(not(target_os = "linux"))]
    bail!("fail-closed: execution restrictions currently require Linux");
    if cancelled() {
        bail!("request cancelled")
    }
    let start = Instant::now();
    let mut child = command.spawn()?;
    let stdout = child.stdout.take().unwrap();
    let stderr = child.stderr.take().unwrap();
    let out = std::thread::spawn(move || capture(stdout));
    let err = std::thread::spawn(move || capture(stderr));
    let mut failure = None;
    let status = loop {
        if let Some(status) = child.try_wait()? {
            break status;
        }
        if cancelled() || start.elapsed() > Duration::from_secs(seconds) {
            failure = Some(if cancelled() { "cancelled" } else { "timeout" }.into());
            unsafe {
                libc::kill(-(child.id() as i32), libc::SIGKILL);
            }
            break child.wait()?;
        }
        std::thread::sleep(Duration::from_millis(10));
    };
    // Kill descendants before joining output readers, including after normal exit.
    unsafe {
        libc::kill(-(child.id() as i32), libc::SIGKILL);
    }
    let (stdout, ot) = out
        .join()
        .map_err(|_| anyhow::anyhow!("output reader panicked"))?;
    let (stderr, et) = err
        .join()
        .map_err(|_| anyhow::anyhow!("output reader panicked"))?;
    if ot || et {
        failure = Some("output_limit".into())
    } else if !status.success() && failure.is_none() {
        failure = Some("nonzero_exit".into())
    }
    static OUTPUT_SEQUENCE: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(0);
    let raw = write_dir.join("raw");
    // Concurrent executions may race to create the shared output directory.
    match std::fs::create_dir(&raw) {
        Err(e) if e.kind() != std::io::ErrorKind::AlreadyExists => return Err(e.into()),
        _ => {}
    }
    let sequence = OUTPUT_SEQUENCE.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
    let output_refs = vec![
        format!("raw/exec-{sequence}.stdout"),
        format!("raw/exec-{sequence}.stderr"),
    ];
    crate::evidence::write_new(&write_dir.join(&output_refs[0]), &stdout)?;
    crate::evidence::write_new(&write_dir.join(&output_refs[1]), &stderr)?;
    let stdout = String::from_utf8_lossy(&stdout).into_owned();
    let stderr = String::from_utf8_lossy(&stderr).into_owned();
    Ok(Execution {
        output_refs,metadata_output_truncated:false,
        executable_sha256,
        executable: exe.display().to_string(),
        arguments: args.to_vec(),
        cwd: cwd.display().to_string(),
        environment: env,
        exit_code: status.code(),
        failure,
        duration_ms: start.elapsed().as_millis(),
        stdout,
        stderr,
        timeout_seconds: seconds,
        restrictions: if unsafe {libc::syscall(libc::SYS_landlock_create_ruleset, 0, 0, 1)} >= 3 {"network denied; filesystem writes restricted to evidence"} else {"network denied; filesystem isolation unavailable; trusted read-only analyzer invocation"}.into(),
    })
}
#[cfg(target_os = "linux")]
fn isolate(output: &std::ffi::CStr, require_filesystem_isolation: bool) -> std::io::Result<()> {
    // Landlock restricts filesystem writes to evidence; seccomp denies socket creation.
    // This is not a general sandbox: reads of system toolchains remain available.
    unsafe {
        if libc::prctl(
            libc::PR_SET_NO_NEW_PRIVS,
            1 as libc::c_ulong,
            0 as libc::c_ulong,
            0 as libc::c_ulong,
            0 as libc::c_ulong,
        ) != 0
        {
            return Err(std::io::Error::last_os_error());
        }
        let abi = libc::syscall(libc::SYS_landlock_create_ruleset, 0, 0, 1);
        if abi < 3 && require_filesystem_isolation {
            return Err(std::io::Error::from_raw_os_error(libc::ENOSYS));
        }
        if abi >= 3 {
            let access: u64 = (1 << 1)
                | (1 << 4)
                | (1 << 5)
                | (1 << 6)
                | (1 << 7)
                | (1 << 8)
                | (1 << 9)
                | (1 << 10)
                | (1 << 11)
                | (1 << 12)
                | (1 << 13)
                | (1 << 14);
            let ruleset = libc::syscall(libc::SYS_landlock_create_ruleset, &access, 8, 0) as i32;
            if ruleset < 0 {
                return Err(std::io::Error::last_os_error());
            }
            #[repr(C, packed)]
            struct Rule {
                allowed: u64,
                parent: i32,
            }
            let parent = libc::open(output.as_ptr(), libc::O_PATH | libc::O_CLOEXEC);
            let rule = Rule {
                allowed: access,
                parent,
            };
            let mut add = libc::syscall(libc::SYS_landlock_add_rule, ruleset, 1, &rule, 0);
            // Tools such as git open /dev/null read-write; permit only file
            // writes and truncation on that device, nothing else outside evidence.
            let null = libc::open(c"/dev/null".as_ptr(), libc::O_PATH | libc::O_CLOEXEC);
            if add == 0 && null >= 0 {
                let rule = Rule {
                    allowed: (1 << 1) | (1 << 14),
                    parent: null,
                };
                add = libc::syscall(libc::SYS_landlock_add_rule, ruleset, 1, &rule, 0);
            }
            if null >= 0 {
                libc::close(null);
            }
            let restrict = if add == 0 {
                libc::syscall(libc::SYS_landlock_restrict_self, ruleset, 0)
            } else {
                -1
            };
            libc::close(parent);
            libc::close(ruleset);
            if restrict != 0 {
                return Err(std::io::Error::last_os_error());
            }
        }
        #[cfg(target_arch = "x86_64")]
        let architecture = 0xc000003e;
        #[cfg(target_arch = "aarch64")]
        let architecture = 0xc00000b7;
        let mut filters = [
            libc::sock_filter {
                code: 0x20,
                jt: 0,
                jf: 0,
                k: 4,
            },
            libc::sock_filter {
                code: 0x15,
                jt: 1,
                jf: 0,
                k: architecture,
            },
            libc::sock_filter {
                code: 0x06,
                jt: 0,
                jf: 0,
                k: 0x80000000,
            },
            libc::sock_filter {
                code: 0x20,
                jt: 0,
                jf: 0,
                k: 0,
            },
            libc::sock_filter {
                code: 0x35,
                jt: 0,
                jf: 1,
                k: 0x40000000,
            },
            libc::sock_filter {
                code: 0x06,
                jt: 0,
                jf: 0,
                k: 0x80000000,
            },
            libc::sock_filter {
                code: 0x15,
                jt: 0,
                jf: 1,
                k: libc::SYS_io_uring_setup as u32,
            },
            libc::sock_filter {
                code: 0x06,
                jt: 0,
                jf: 0,
                k: 0x00050000 | libc::EPERM as u32,
            },
            libc::sock_filter {
                code: 0x20,
                jt: 0,
                jf: 0,
                k: 0,
            },
            libc::sock_filter {
                code: 0x15,
                jt: 2,
                jf: 0,
                k: libc::SYS_socket as u32,
            },
            libc::sock_filter {
                code: 0x15,
                jt: 1,
                jf: 0,
                k: libc::SYS_socketpair as u32,
            },
            libc::sock_filter {
                code: 0x06,
                jt: 0,
                jf: 0,
                k: 0x7fff0000,
            },
            libc::sock_filter {
                code: 0x06,
                jt: 0,
                jf: 0,
                k: 0x00050000 | libc::EPERM as u32,
            },
        ];
        let program = libc::sock_fprog {
            len: filters.len() as u16,
            filter: filters.as_mut_ptr(),
        };
        if libc::prctl(
            libc::PR_SET_SECCOMP,
            2 as libc::c_ulong,
            &program as *const libc::sock_fprog,
        ) != 0
        {
            return Err(std::io::Error::last_os_error());
        }
    }
    Ok(())
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn output_bound() {
        let bytes = vec![b'x'; OUTPUT_LIMIT + 10];
        let (s, t) = capture(bytes.as_slice());
        assert!(t);
        assert_eq!(s.len(), OUTPUT_LIMIT);
    }
    #[test]
    fn real_execution() {
        let d = tempfile::tempdir().unwrap();
        let e = execute(
            Path::new("/usr/bin/true"),
            &[],
            d.path(),
            d.path(),
            2,
            256,
            false,
        )
        .unwrap();
        assert_eq!(e.exit_code, Some(0));
    }
    #[test]
    fn timeout() {
        let d = tempfile::tempdir().unwrap();
        let e = execute(
            Path::new("/usr/bin/sleep"),
            &["5".into()],
            d.path(),
            d.path(),
            1,
            256,
            false,
        )
        .unwrap();
        assert_eq!(e.failure.as_deref(), Some("timeout"));
    }
    #[test]
    fn filesystem_capability_is_reported() {
        let d = tempfile::tempdir().unwrap();
        let e = execute(
            Path::new("/usr/bin/true"),
            &[],
            d.path(),
            d.path(),
            2,
            256,
            false,
        )
        .unwrap();
        assert!(e.restrictions.contains("network denied"));
    }

    #[test]
    fn network_denied() {
        let d = tempfile::tempdir().unwrap();
        let e = execute(
            Path::new("/usr/bin/python3"),
            &["-c".into(), "import socket; socket.socket()".into()],
            d.path(),
            d.path(),
            2,
            256,
            false,
        )
        .unwrap();
        assert!(e.stderr.contains("Operation not permitted"), "{}", e.stderr);
        assert_ne!(e.exit_code, Some(0));
    }
    #[test]
    fn parallel_keeps_order_and_cancellation() {
        let items: Vec<usize> = (0..50).collect();
        let squares = parallel(&items, 4, |i, n| (i, n * n));
        assert!(
            squares
                .iter()
                .enumerate()
                .all(|(i, r)| *r == Some((i, i * i)))
        );
        let seen = std::sync::atomic::AtomicUsize::new(0);
        let start = Instant::now();
        let partial = with_cancellation(
            move || start.elapsed() > Duration::from_millis(50),
            || {
                parallel(&items, 2, |_, _| {
                    seen.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                    std::thread::sleep(Duration::from_millis(20))
                })
            },
        );
        assert!(partial.iter().any(Option::is_none));
        assert_eq!(
            partial.iter().filter(|r| r.is_some()).count(),
            seen.into_inner()
        );
    }
    #[test]
    fn cancellation_kills_child() {
        let d = tempfile::tempdir().unwrap();
        let start = Instant::now();
        let e = with_cancellation(
            move || start.elapsed() > Duration::from_millis(100),
            || {
                execute(
                    Path::new("/usr/bin/sleep"),
                    &["5".into()],
                    d.path(),
                    d.path(),
                    10,
                    256,
                    false,
                )
            },
        )
        .unwrap();
        assert_eq!(e.failure.as_deref(), Some("cancelled"));
    }
    #[test]
    fn isolated_tools_can_write_dev_null() {
        let out = tempfile::tempdir().unwrap();
        let e = execute(
            Path::new("/usr/bin/sh"),
            &[
                "-c".into(),
                "echo x > /dev/null && exec 3<>/dev/null".into(),
            ],
            out.path(),
            out.path(),
            2,
            256,
            false,
        )
        .unwrap();
        assert_eq!(e.exit_code, Some(0), "{}", e.stderr);
    }
    #[test]
    fn strict_isolation_fails_closed_or_blocks_writes() {
        let source = tempfile::tempdir().unwrap();
        let out = tempfile::tempdir().unwrap();
        let result = execute(
            Path::new("/usr/bin/touch"),
            &[source.path().join("forbidden").display().to_string()],
            source.path(),
            out.path(),
            2,
            256,
            true,
        );
        if unsafe { libc::syscall(libc::SYS_landlock_create_ruleset, 0, 0, 1) } < 3 {
            assert!(result.is_err())
        } else {
            assert_ne!(result.unwrap().exit_code, Some(0))
        }
        assert!(!source.path().join("forbidden").exists());
    }
}
