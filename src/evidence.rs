use crate::{
    config::{Config, inside, output_dir},
    model::{Completeness, Run, Status},
};
use anyhow::{Result, bail};
use sha2::{Digest, Sha256};
use std::{
    collections::BTreeMap,
    fs,
    path::{Path, PathBuf},
    time::{SystemTime, UNIX_EPOCH},
};
pub fn now() -> u128 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap_or_default()
        .as_millis()
}
pub fn hash(bytes: &[u8]) -> String {
    format!("{:x}", Sha256::digest(bytes))
}
pub fn hash_file(path: &Path) -> Result<String> {
    use std::io::Read;
    let mut file = fs::File::open(path)?;
    let mut hasher = Sha256::new();
    let mut bytes = [0u8; 65536];
    loop {
        anyhow::ensure!(
            !crate::runner::cancelled(),
            "request cancelled during hashing"
        );
        let n = file.read(&mut bytes)?;
        if n == 0 {
            break;
        }
        hasher.update(&bytes[..n]);
    }
    Ok(format!("{:x}", hasher.finalize()))
}
pub fn fingerprint(root: &Path, excluded: &Path) -> Result<BTreeMap<String, String>> {
    fn walk(
        root: &Path,
        dir: &Path,
        excluded: &Path,
        out: &mut BTreeMap<String, String>,
    ) -> Result<()> {
        let mut entries = fs::read_dir(dir)?.collect::<std::io::Result<Vec<_>>>()?;
        entries.sort_by_key(|e| e.file_name());
        for e in entries {
            anyhow::ensure!(
                !crate::runner::cancelled(),
                "request cancelled during fingerprinting"
            );
            let p = e.path();
            let rel = p.strip_prefix(root)?;
            if p.starts_with(excluded)
                || rel
                    .components()
                    .next()
                    .is_some_and(|c| c.as_os_str() == ".git")
            {
                continue;
            }
            let meta = fs::symlink_metadata(&p)?;
            if meta.file_type().is_symlink() {
                bail!(
                    "input symlinks require explicit resolution: {}",
                    p.display()
                )
            }
            if meta.is_dir() {
                walk(root, &p, excluded, out)?
            } else if meta.is_file() {
                out.insert(rel.to_string_lossy().into_owned(), hash_file(&p)?);
            }
        }
        Ok(())
    }
    let mut out = BTreeMap::new();
    walk(root, root, excluded, &mut out)?;
    Ok(out)
}
pub fn begin(root: &Path, c: &Config, operation: &str) -> Result<(Run, PathBuf)> {
    let base = output_dir(root, &c.evidence.directory)?;
    static NEXT: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(0);
    let id = format!(
        "RUN-{}-{}-{}",
        now(),
        std::process::id(),
        NEXT.fetch_add(1, std::sync::atomic::Ordering::Relaxed)
    );
    let dir = base.join(&id);
    fs::create_dir(&dir)?;
    fs::create_dir(dir.join("raw"))?;
    let inputs = fingerprint(root, &base)?;
    write_new(&dir.join("config.json"), &serde_json::to_vec_pretty(c)?)?;
    if c.coding.c.is_some() || c.coding.cpp.is_some() {
        write_new(
            &dir.join("misra-drafts.json"),
            &serde_json::to_vec_pretty(&crate::misra::drafts(c))?,
        )?;
    }
    let mut run=Run{schema_version:1,configuration:serde_json::to_value(c)?,id,operation:operation.into(),project:c.project.name.clone(),status:Status::Incomplete,complete:false,completeness:Completeness::default(),started_unix_ms:now(),completed_unix_ms:0,inputs,executions:Vec::new(),findings:Vec::new(),properties:Vec::new(),limitations:vec!["Automated evidence for human review; no certification or compliance approval.".into(),"MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.".into(),"System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.".into()],git:None};
    if let Some(git) = crate::discovery::executable("git", &c.tools.allowlist) {
        let mut values = serde_json::Map::new();
        for (name, args) in [
            ("commit", vec!["rev-parse".to_string(), "HEAD".into()]),
            (
                "dirty",
                vec![
                    "--no-optional-locks".into(),
                    "-c".into(),
                    "core.fsmonitor=false".into(),
                    "status".into(),
                    "--porcelain=v1".into(),
                    "--untracked-files=all".into(),
                ],
            ),
        ] {
            match crate::runner::execute(
                &git,
                &args,
                root,
                &dir,
                10,
                c.tools.memory_mb,
                c.tools.require_filesystem_isolation,
            ) {
                Ok(e) => {
                    if e.failure.is_none() {
                        values.insert(
                            name.into(),
                            if name == "dirty" {
                                serde_json::json!(!e.stdout.trim().is_empty())
                            } else {
                                serde_json::json!(e.stdout.trim())
                            },
                        );
                    }
                    run.executions.push(e);
                }
                Err(e) => run
                    .limitations
                    .push(format!("Git metadata unavailable: {e}")),
            }
        }
        if !values.is_empty() {
            run.git = Some(serde_json::Value::Object(values));
        }
    }
    Ok((run, dir))
}
pub fn finish(root: &Path, c: &Config, dir: &Path, mut run: Run) -> Result<Run> {
    let after = fingerprint(root, &root.join(&c.evidence.directory))?;
    if after != run.inputs {
        run.status = Status::Incomplete;
        run.complete = false;
        run.limitations
            .push("Inputs changed during execution".into());
    }
    run.completed_unix_ms = now();
    write_new(
        &dir.join("findings.json"),
        &serde_json::to_vec_pretty(&run.findings)?,
    )?;
    write_new(
        &dir.join("manifest.json"),
        &serde_json::to_vec_pretty(&run)?,
    )?;
    write_new(
        &dir.join("report.md"),
        crate::report::markdown(&run).as_bytes(),
    )?;
    let mut hashes = BTreeMap::new();
    collect_hashes(dir, dir, &mut hashes)?;
    write_new(
        &dir.join("evidence.json"),
        &serde_json::to_vec_pretty(&hashes)?,
    )?;
    Ok(run)
}
fn collect_hashes(root: &Path, dir: &Path, out: &mut BTreeMap<String, String>) -> Result<()> {
    for e in fs::read_dir(dir)? {
        let p = e?.path();
        let m = fs::symlink_metadata(&p)?;
        if m.file_type().is_symlink() {
            bail!("evidence symlink")
        };
        if m.is_dir() {
            collect_hashes(root, &p, out)?
        } else {
            out.insert(
                p.strip_prefix(root)?.to_string_lossy().into_owned(),
                hash_file(&p)?,
            );
        }
    }
    Ok(())
}
pub fn write_new(path: &Path, data: &[u8]) -> Result<()> {
    use std::io::Write;
    let mut f = fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(path)?;
    f.write_all(data)?;
    f.sync_all()?;
    Ok(())
}
pub fn load(root: &Path, c: &Config, id: &str) -> Result<Run> {
    if !id.starts_with("RUN-") || !id.chars().all(|c| c.is_ascii_alphanumeric() || c == '-') {
        bail!("invalid run ID")
    }
    let dir = inside(root, &c.evidence.directory.join(id))?;
    let hashes: BTreeMap<String, String> =
        serde_json::from_slice(&fs::read(dir.join("evidence.json"))?)?;
    let mut actual = BTreeMap::new();
    collect_hashes(&dir, &dir, &mut actual)?;
    actual.remove("evidence.json");
    if actual != hashes {
        bail!("evidence integrity verification failed")
    }
    Ok(serde_json::from_slice(&fs::read(
        dir.join("manifest.json"),
    )?)?)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn dirty_files_hashed() {
        let t = tempfile::tempdir().unwrap();
        fs::write(t.path().join("untracked.c"), "a").unwrap();
        let a = fingerprint(t.path(), &t.path().join(".safety")).unwrap();
        fs::write(t.path().join("untracked.c"), "b").unwrap();
        assert_ne!(a, fingerprint(t.path(), &t.path().join(".safety")).unwrap());
    }
    #[test]
    fn tamper() {
        let t = tempfile::tempdir().unwrap();
        let c = Config::new("x".into());
        let (r, d) = begin(t.path(), &c, "test").unwrap();
        let r = finish(t.path(), &c, &d, r).unwrap();
        load(t.path(), &c, &r.id).unwrap();
        fs::write(d.join("findings.json"), "[] altered").unwrap();
        assert!(load(t.path(), &c, &r.id).is_err());
    }
}
