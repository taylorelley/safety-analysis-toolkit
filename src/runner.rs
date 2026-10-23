thread_local! {static CANCEL:std::cell::RefCell<Option<Box<dyn Fn()->bool>>>=std::cell::RefCell::new(None);}
pub fn with_cancellation<T>(check: impl Fn() -> bool + 'static, work: impl FnOnce() -> T) -> T {
    CANCEL.with(|c| *c.borrow_mut() = Some(Box::new(check)));
    struct Reset;
    impl Drop for Reset {
        fn drop(&mut self) {
            CANCEL.with(|c| *c.borrow_mut() = None);
        }
    }
    let _reset = Reset;
    work()
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
    if !raw.exists() {
        std::fs::create_dir(&raw)?;
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
            let add = libc::syscall(libc::SYS_landlock_add_rule, ruleset, 1, &rule, 0);
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
