use crate::config::inside;
use anyhow::{Context, Result, bail};
use serde::{Deserialize, Serialize};
use sha2::{Digest, Sha256};
use std::{
    collections::BTreeSet,
    fs,
    path::{Path, PathBuf},
};
#[derive(Debug, Deserialize)]
struct Entry {
    directory: PathBuf,
    file: PathBuf,
    arguments: Option<Vec<String>>,
    command: Option<String>,
}
#[derive(Debug, Serialize, Clone)]
pub struct Unit {
    pub file: PathBuf,
    pub directory: PathBuf,
    pub compiler: String,
    pub arguments: Vec<String>,
    pub variant: String,
    pub unsupported: Vec<String>,
    pub include_directories: Vec<String>,
    pub definitions: Vec<String>,
    pub dialect: Option<String>,
    pub target: Option<String>,
    pub compiler_extensions: Vec<String>,
}
pub fn database(root: &Path, path: &Path) -> Result<Vec<Unit>> {
    let path = inside(root, path)?;
    let entries: Vec<Entry> = serde_json::from_slice(&fs::read(path)?)?;
    if entries.is_empty() {
        bail!("empty compilation database")
    }
    let mut units = Vec::new();
    let mut seen = BTreeSet::new();
    for e in entries {
        let directory = inside(root, &e.directory)?;
        let file = inside(root, &directory.join(&e.file))?;
        if !["c", "cc", "cpp", "cxx", "C"]
            .contains(&file.extension().and_then(|x| x.to_str()).unwrap_or(""))
        {
            bail!("unsupported source extension: {}", file.display())
        }
        let mut args = match e.arguments {
            Some(a) => a,
            None => shlex::split(&e.command.context("missing arguments or command")?)
                .context("invalid command quoting")?,
        };
        if args.is_empty() {
            bail!("empty compilation command")
        }
        let compiler = args.remove(0);
        let key = serde_json::to_vec(&(&file, &directory, &compiler, &args))?;
        let variant = format!("VAR-{:x}", Sha256::digest(&key));
        if !seen.insert(variant.clone()) {
            continue;
        }
        let mut unsupported = validate_args(&args);
        let driver = Path::new(&compiler)
            .file_name()
            .and_then(|s| s.to_str())
            .unwrap_or("");
        let known = ["cc", "c++", "clang", "clang++", "gcc", "g++"]
            .iter()
            .any(|name| {
                driver == *name
                    || driver
                        .strip_prefix(&format!("{name}-"))
                        .is_some_and(|suffix| {
                            suffix.chars().all(|c| c.is_ascii_digit() || c == '.')
                        })
            });
        if !known {
            unsupported.push(format!("compiler semantics not reviewed: {compiler}"));
        }
        let mut include_directories = Vec::new();
        let mut definitions = Vec::new();
        let mut dialect = None;
        let mut target = None;
        let mut compiler_extensions = Vec::new();
        let mut iter = args.iter();
        while let Some(arg) = iter.next() {
            match arg.as_str() {
                "-I" | "-isystem" | "-iquote" => {
                    if let Some(v) = iter.next() {
                        include_directories.push(v.clone());
                    }
                }
                "-D" => {
                    if let Some(v) = iter.next() {
                        definitions.push(v.clone());
                    }
                }
                "-target" | "--target" => {
                    target = iter.next().cloned();
                }
                _ => {
                    if let Some(v) = arg.strip_prefix("-I") {
                        include_directories.push(v.into())
                    } else if let Some(v) = arg.strip_prefix("-D") {
                        definitions.push(v.into())
                    } else if let Some(v) = arg.strip_prefix("-std=") {
                        dialect = Some(v.into())
                    } else if let Some(v) = arg.strip_prefix("--target=") {
                        target = Some(v.into())
                    } else if arg.starts_with("-f") || arg.starts_with("-m") {
                        compiler_extensions.push(arg.clone())
                    }
                }
            }
        }
        units.push(Unit {
            file,
            directory,
            compiler,
            arguments: args,
            variant,
            unsupported,
            include_directories,
            definitions,
            dialect,
            target,
            compiler_extensions,
        });
    }
    Ok(units)
}
// No flags are dropped. Unknown flags make the unit unsupported. Compiler paths
// are recorded but never executed. Project response files and plugins are denied.
fn validate_args(args: &[String]) -> Vec<String> {
    let mut problems = Vec::new();
    let mut value = false;
    for arg in args {
        if value {
            value = false;
            continue;
        }
        if [
            "-I",
            "-isystem",
            "-iquote",
            "-D",
            "-U",
            "-o",
            "-include",
            "-imacros",
            "-target",
            "--target",
            "-x",
            "-isysroot",
            "--sysroot",
        ]
        .contains(&arg.as_str())
        {
            value = true;
            continue;
        }
        let exact = [
            "-c",
            "-g",
            "-pipe",
            "-pthread",
            "-fno-exceptions",
            "-fexceptions",
            "-fno-rtti",
            "-frtti",
            "-fshort-enums",
            "-fshort-wchar",
            "-fsigned-char",
            "-funsigned-char",
            "-ffreestanding",
            "-fno-builtin",
            "-fprofile-instr-generate",
            "-fcoverage-mapping",
            "-fcoverage-mcdc",
            "-m32",
            "-m64",
            "-O0",
            "-O1",
            "-O2",
            "-O3",
            "-Os",
            "-Og",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic",
        ];
        let prefix = [
            "-I",
            "-D",
            "-U",
            "-std=",
            "--target=",
            "--sysroot=",
            "-mcpu=",
            "-march=",
            "-mfloat-abi=",
            "-mfpu=",
        ];
        if exact.contains(&arg.as_str()) || prefix.iter().any(|p| arg.starts_with(p)) {
            continue;
        }
        if !arg.starts_with('-')
            && !arg.starts_with('@')
            && [".c", ".cc", ".cpp", ".cxx", ".C"]
                .iter()
                .any(|s| arg.ends_with(s))
        {
            continue;
        }
        problems.push(format!("unreviewed compiler option: {arg}"));
    }
    if value {
        problems.push("missing compiler option value".into())
    }
    problems
}
pub fn executable(name: &str, allowlist: &[PathBuf]) -> Option<PathBuf> {
    let candidate = std::env::var_os("PATH").and_then(|v| {
        std::env::split_paths(&v)
            .map(|p| p.join(name))
            .find(|p| p.is_file())
    })?;
    let p = candidate.canonicalize().ok()?;
    if allowlist.is_empty() {
        // Default allowlist is the fixed set of supported tool names.
        if ![
            "clang-tidy-19",
            "clang-19",
            "llvm-cov-19",
            "llvm-profdata-19",
            "frama-c",
            "esbmc",
            "safety-source-index",
            "git",
        ]
        .contains(&name)
        {
            return None;
        }
    } else if !allowlist
        .iter()
        .any(|a| a.canonicalize().ok().as_ref() == Some(&p))
    {
        return None;
    }
    Some(p)
}
pub fn doctor(root: &Path, c: &crate::config::Config) -> Result<crate::model::Run> {
    use crate::{discovery, evidence, model::Status, runner};
    let (mut run, dir) = evidence::begin(root, c, "doctor")?;
    match discovery::database(root, &c.build.compile_commands) {
        Ok(units) => {
            run.completeness.total = units.len();
            run.completeness.build_variants = units.iter().map(|u| u.variant.clone()).collect();
            for u in &units {
                if u.unsupported.is_empty() {
                    run.completeness.analyzed += 1
                } else {
                    run.completeness.unsupported += 1;
                    run.limitations.extend(u.unsupported.clone())
                }
            }
            evidence::write_new(&dir.join("units.json"), &serde_json::to_vec_pretty(&units)?)?;
        }
        Err(e) => run
            .limitations
            .push(format!("Compilation database unavailable: {e}")),
    }
    let mut missing = false;
    for (name, required) in [
        ("clang-tidy-19", c.tools.clang),
        ("clang-19", false),
        ("llvm-cov-19", false),
        ("llvm-profdata-19", false),
        ("frama-c", c.tools.frama_c),
        ("esbmc", c.tools.esbmc),
    ] {
        if let Some(exe) = discovery::executable(name, &c.tools.allowlist) {
            match runner::execute(
                &exe,
                &[if name == "frama-c" {
                    "-version".into()
                } else {
                    "--version".into()
                }],
                root,
                &dir,
                c.tools.timeout_seconds,
                c.tools.memory_mb,
                c.tools.require_filesystem_isolation,
            ) {
                Ok(e) => {
                    missing |= required && e.failure.is_some();
                    run.executions.push(e)
                }
                Err(e) => {
                    missing |= required;
                    run.limitations.push(format!("{name}: {e}"));
                }
            }
        } else {
            missing |= required;
            run.limitations.push(format!(
                "{name}: unavailable or not allowlisted ({})",
                if required { "required" } else { "optional" }
            ));
        }
    }
    run.complete = run.completeness.complete() && !missing;
    run.status = if run.complete {
        Status::Passed
    } else {
        Status::Incomplete
    };
    run.limitations
        .push("Doctor establishes discovery only; it does not analyze source.".into());
    evidence::finish(root, c, &dir, run)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn flags() {
        assert!(
            validate_args(&[
                "-std=c11".into(),
                "-DVAR=1".into(),
                "-c".into(),
                "x.c".into()
            ])
            .is_empty()
        );
        for f in ["@evil", "-fplugin=evil", "-Xclang", "-funknown"] {
            assert!(!validate_args(&[f.into()]).is_empty());
        }
    }
    #[test]
    fn variants() {
        let d = tempfile::tempdir().unwrap();
        fs::write(d.path().join("x.c"), "int x;").unwrap();
        let entries = serde_json::json!([{"directory":d.path(),"file":"x.c","arguments":["cc","-DA=1","-c","x.c"]},{"directory":d.path(),"file":"x.c","arguments":["cc","-DA=2","-c","x.c"]}]);
        fs::write(d.path().join("compile_commands.json"), entries.to_string()).unwrap();
        let units = database(d.path(), Path::new("compile_commands.json")).unwrap();
        assert_eq!(units.len(), 2);
        assert_ne!(units[0].variant, units[1].variant);
    }
}
