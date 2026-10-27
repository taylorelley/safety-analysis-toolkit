use crate::config::inside;
use anyhow::{Context, Result, bail};
use serde::{Deserialize, Serialize};
use sha2::{Digest, Sha256};
use std::{
    collections::BTreeSet,
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
    let entries: Vec<Entry> =
        serde_json::from_slice(&crate::config::read_bounded(&path, 32 * 1024 * 1024)?)?;
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
/// Dotted numeric version following `marker` in tool output, e.g. "version 19.1.1".
pub fn version_after(text: &str, marker: &str) -> Option<Vec<u32>> {
    let rest = &text[text.find(marker)? + marker.len()..];
    let end = rest
        .find(|c: char| !c.is_ascii_digit() && c != '.')
        .unwrap_or(rest.len());
    rest[..end]
        .trim_end_matches('.')
        .split('.')
        .map(|part| part.parse().ok())
        .collect()
}
/// Validated reference releases. Other supported releases run, but each run
/// records that output interpretation for that release is unvalidated.
pub const LLVM_REFERENCE: &str = "19.1.7";
pub const ESBMC_REFERENCE: &str = "8.6.0";
pub const FRAMA_C_REFERENCE: &str = "25.0-beta (Manganese)";
/// Supported LLVM release: any 19.1.x patch release, matching LLVM's own
/// major.minor compatibility rule for the native module.
pub fn llvm_release(version_output: &str) -> Option<String> {
    version_after(version_output, "version ")
        .filter(|v| v.len() == 3 && v.starts_with(&[19, 1]))
        .map(|v| v.iter().map(u32::to_string).collect::<Vec<_>>().join("."))
}
/// Limitation for a supported release that is not the validated reference.
/// Releases are compared as full strings, so 25.0 and 25.0-beta differ.
pub fn reference_note(tool: &str, release: &str, reference: &str) -> Option<String> {
    (release != reference).then(|| {
        format!(
            "{tool} {release} differs from the validated reference {reference}; output interpretation for this release is unvalidated"
        )
    })
}
/// Tool names resolved from PATH when no explicit allowlist is configured.
pub const SUPPORTED_TOOLS: [&str; 8] = [
    "clang-tidy-19",
    "clang-19",
    "llvm-cov-19",
    "llvm-profdata-19",
    "frama-c",
    "esbmc",
    "safety-source-index",
    "git",
];
pub fn executable(name: &str, allowlist: &[PathBuf]) -> Option<PathBuf> {
    let candidate = std::env::var_os("PATH").and_then(|v| {
        std::env::split_paths(&v)
            .map(|p| p.join(name))
            .find(|p| p.is_file())
    })?;
    let p = candidate.canonicalize().ok()?;
    if allowlist.is_empty() {
        // Default allowlist is the fixed set of supported tool names.
        if !SUPPORTED_TOOLS.contains(&name) {
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
/// Frama-C plugin names listed by `frama-c -plugins`: (Eva, WP).
fn frama_c_plugins(listing: &str) -> (bool, bool) {
    let has = |name: &str| {
        listing
            .lines()
            .any(|l| l.split_whitespace().next() == Some(name))
    };
    (has("Eva"), has("WP"))
}
/// Whether `esbmc --list-solvers` names Z3, the solver the adapter selects.
fn esbmc_has_z3(listing: &str) -> bool {
    listing
        .lines()
        .filter_map(|l| l.strip_prefix("Available solvers:"))
        .any(|l| l.split_whitespace().any(|s| s == "z3"))
}
/// Version gates and capability probes for a discovered tool, mirroring what
/// scan, prove and coverage require. Returns (problem, fatal-if-required).
fn capabilities(
    name: &str,
    exe: &Path,
    banner: &str,
    root: &Path,
    dir: &Path,
    c: &crate::config::Config,
    run: &mut crate::model::Run,
) -> Vec<(String, bool)> {
    let mut probe = |args: &[&str]| {
        let args = args.iter().map(|a| a.to_string()).collect::<Vec<_>>();
        match crate::runner::execute(
            exe,
            &args,
            root,
            dir,
            c.tools.timeout_seconds,
            c.tools.memory_mb,
            c.tools.require_filesystem_isolation,
        ) {
            Ok(e) => {
                // Some tools (ESBMC --list-solvers) report on stderr.
                let out = (e.failure.is_none()).then(|| format!("{}\n{}", e.stdout, e.stderr));
                run.record(e);
                out
            }
            Err(_) => None,
        }
    };
    let mut problems = Vec::new();
    match name {
        "clang-tidy-19" | "llvm-cov-19" => match llvm_release(banner) {
            None => problems.push((
                "requires LLVM 19.1.0 or a later 19.1.x release".into(),
                true,
            )),
            Some(release) => {
                if let Some(note) = reference_note("LLVM", &release, LLVM_REFERENCE) {
                    problems.push((note, false));
                }
                if name == "clang-tidy-19"
                    && let Some(module) = &c.tools.clang_module
                {
                    let checks = probe(&[
                        &format!("--load={}", module.display()),
                        "--checks=-*,misra-c2012-*,safety-*",
                        "--list-checks",
                    ]);
                    let loaded = checks
                        .as_deref()
                        .is_some_and(|out| out.contains("misra-c2012-") && out.contains("safety-"));
                    if !loaded {
                        problems.push((
                            "configured clang_module did not load or registered no misra-c2012/safety checks; build it against this clang-tidy".into(),
                            true,
                        ));
                    }
                }
            }
        },
        "esbmc" => match version_after(banner, "ESBMC version ") {
            Some(v) if v >= vec![8, 6, 0] => {
                let release = v.iter().map(u32::to_string).collect::<Vec<_>>().join(".");
                if let Some(note) = reference_note("ESBMC", &release, ESBMC_REFERENCE) {
                    problems.push((note, false));
                }
                if !probe(&["--list-solvers"])
                    .as_deref()
                    .is_some_and(esbmc_has_z3)
                {
                    problems.push((
                        "Z3 solver unavailable; the ESBMC adapter requires Z3".into(),
                        true,
                    ));
                }
            }
            _ => problems.push(("requires ESBMC 8.6.0 or later".into(), true)),
        },
        "frama-c" => match version_after(banner.trim(), "") {
            Some(v) if v >= vec![25] => {
                if let Some(note) = reference_note("Frama-C", banner.trim(), FRAMA_C_REFERENCE) {
                    problems.push((note, false));
                }
                let (eva, wp) = probe(&["-plugins"])
                    .as_deref()
                    .map(frama_c_plugins)
                    .unwrap_or_default();
                if !eva {
                    problems.push((
                        "Eva plugin unavailable; prove --engine frama-c cannot run".into(),
                        true,
                    ));
                }
                if !wp {
                    problems.push((
                        "WP plugin unavailable; prove --method wp will be incomplete (some distribution packages omit WP)".into(),
                        false,
                    ));
                }
            }
            _ => problems.push(("requires Frama-C 25.0 or later".into(), true)),
        },
        _ => {}
    }
    problems
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
                    let banner = (e.failure.is_none()).then(|| e.stdout.clone());
                    run.record(e);
                    if let Some(banner) = banner {
                        for (problem, fatal) in
                            capabilities(name, &exe, &banner, root, &dir, c, &mut run)
                        {
                            missing |= required && fatal;
                            run.limitations.push(format!("{name}: {problem}"));
                        }
                    }
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
    use std::fs;
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
    fn versions() {
        assert_eq!(
            llvm_release("Ubuntu LLVM version 19.1.7\n").as_deref(),
            Some("19.1.7")
        );
        assert_eq!(
            llvm_release("LLVM version 19.1.8 (x)").as_deref(),
            Some("19.1.8")
        );
        assert_eq!(
            llvm_release("Ubuntu LLVM version 19.1.1").as_deref(),
            Some("19.1.1")
        );
        assert_eq!(
            llvm_release("LLVM version 19.1.0").as_deref(),
            Some("19.1.0")
        );
        assert_eq!(llvm_release("LLVM version 19.2.0"), None);
        assert_eq!(llvm_release("LLVM version 20.1.0"), None);
        assert_eq!(llvm_release("LLVM version 18.1.8"), None);
        assert_eq!(llvm_release("no version here"), None);
        assert_eq!(reference_note("LLVM", "19.1.7", LLVM_REFERENCE), None);
        assert!(reference_note("LLVM", "19.1.1", LLVM_REFERENCE).is_some());
        assert_eq!(
            reference_note("Frama-C", "25.0-beta (Manganese)", FRAMA_C_REFERENCE),
            None
        );
        assert!(reference_note("Frama-C", "25.0 (Manganese)", FRAMA_C_REFERENCE).is_some());
        assert_eq!(
            version_after("ESBMC version 8.6.0.", "ESBMC version "),
            Some(vec![8, 6, 0])
        );
        assert_eq!(
            version_after("25.0-beta (Manganese)", ""),
            Some(vec![25, 0])
        );
        assert_eq!(version_after("version x", "version "), None);
    }
    #[test]
    fn capability_listings() {
        let full = "Eva                 automatically computes\nWP                  Proof by Weakest Precondition\n";
        assert_eq!(frama_c_plugins(full), (true, true));
        assert_eq!(
            frama_c_plugins("Eva                 automatically computes\nReport  x\n"),
            (true, false)
        );
        assert_eq!(frama_c_plugins("Wpalette x\n"), (false, false));
        assert!(esbmc_has_z3(
            "Target: x\nAvailable solvers: smtlib z3 boolector\nESBMC version 8.6.0"
        ));
        assert!(!esbmc_has_z3("Available solvers: smtlib boolector z3x\n"));
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
