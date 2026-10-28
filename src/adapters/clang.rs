use crate::{
    config::Config,
    discovery, evidence, findings,
    model::{Run, Status},
    runner,
};
use anyhow::{Result, bail};
use std::{
    collections::HashMap,
    fs,
    path::{Path, PathBuf},
};
const CHECKS: &str = "-*,clang-analyzer-*,bugprone-sizeof-expression,bugprone-integer-division,bugprone-suspicious-semicolon";
pub fn scan(root: &Path, c: &Config) -> Result<Run> {
    let units = discovery::database(root, &c.build.compile_commands)?;
    let (mut run, dir) = evidence::begin(root, c, "scan")?;
    run.completeness.total = units.len();
    run.completeness.build_variants = units.iter().map(|u| u.variant.clone()).collect();
    let tool = discovery::executable("clang-tidy-19", &c.tools.allowlist);
    if !c.tools.clang || tool.is_none() {
        run.completeness.failed = units.len();
        run.limitations
            .push("Clang analysis disabled, unavailable, or not allowlisted".into());
        return evidence::finish(root, c, &dir, run);
    }
    let tool = tool.unwrap();
    let version = runner::execute(
        &tool,
        &["--version".into()],
        root,
        &dir,
        c.tools.timeout_seconds,
        c.tools.memory_mb,
        c.tools.require_filesystem_isolation,
    );
    match version {
        Ok(v) => {
            let release = (v.failure.is_none())
                .then(|| discovery::llvm_release(&v.stdout))
                .flatten();
            run.record(v);
            let Some(release) = release else {
                run.completeness.unsupported = units.len();
                run.limitations
                    .push("Requires LLVM 19.1.0 or a later 19.1.x release".into());
                return evidence::finish(root, c, &dir, run);
            };
            run.limitations.extend(discovery::reference_note(
                "LLVM",
                &release,
                discovery::LLVM_REFERENCE,
            ));
        }
        Err(e) => {
            run.completeness.failed = units.len();
            run.limitations.push(format!("Tool startup failure: {e}"));
            return evidence::finish(root, c, &dir, run);
        }
    }
    let module = if let Some(path) = &c.tools.clang_module {
        if !path.is_file() {
            run.completeness.failed = units.len();
            run.limitations
                .push("Configured native module unavailable".into());
            return evidence::finish(root, c, &dir, run);
        }
        let hash = evidence::hash(&fs::read(path)?);
        evidence::write_new(
            &dir.join("module.json"),
            &serde_json::to_vec_pretty(
                &serde_json::json!({"path":path,"sha256":hash,"checks":["safety-prohibited-goto","safety-unsafe-library-call","safety-repeated-macro-parameter","misra-c2012-*"],"misra_mapping":"partial; independent review pending"}),
            )?,
        )?;
        run.limitations
            .push("Original project-policy and MISRA C:2012 checks enabled; MISRA mappings are partial and await independent review".into());
        Some(path)
    } else {
        None
    };
    let analyze = |i: usize, u: &discovery::Unit| -> Result<Outcome> {
        if !u.unsupported.is_empty() {
            return Ok(Outcome::Unsupported);
        }
        let work = dir.join(format!("unit-{i}"));
        fs::create_dir(&work)?;
        let command = serde_json::json!([{"directory":u.directory,"file":u.file,"arguments":std::iter::once(u.compiler.clone()).chain(u.arguments.clone()).collect::<Vec<_>>()}]);
        evidence::write_new(
            &work.join("compile_commands.json"),
            &serde_json::to_vec_pretty(&command)?,
        )?;
        let fixes = work.join("diagnostics.yaml");
        let mut args = vec![
            format!("-p={}", work.display()),
            format!("--checks={CHECKS}"),
            "--config={InheritParentConfig: false, HeaderFilterRegex: '.*'}".into(),
            format!("--export-fixes={}", fixes.display()),
            "--quiet".into(),
            u.file.display().to_string(),
        ];
        if let Some(path) = module {
            args[1] = format!("--checks={CHECKS},safety-*,misra-c2012-*");
            args.insert(0, format!("--load={}", path.display()));
        }
        let mut out = Analyzed::default();
        match runner::execute(
            &tool,
            &args,
            &u.directory,
            &dir,
            c.tools.timeout_seconds,
            c.tools.memory_mb,
            c.tools.require_filesystem_isolation,
        ) {
            Ok(e) => {
                let reference = format!("raw/clang-{i}.log");
                evidence::write_new(
                    &dir.join(&reference),
                    format!("{}\n{}", e.stdout, e.stderr).as_bytes(),
                )?;
                out.failed = e.failure.is_some();
                if fixes.exists() {
                    match findings::parse(&fs::read(&fixes)?, &u.variant, &reference, &u.directory)
                    {
                        Ok(f) => {
                            out.failed |= f.iter().any(|d| d.rule_id == "clang-diagnostic-error");
                            out.findings = f
                        }
                        Err(err) => {
                            out.failed = true;
                            out.limitations.push(err.to_string())
                        }
                    }
                } else if !e.stdout.trim().is_empty() || !e.stderr.trim().is_empty() {
                    out.failed = true;
                    out.limitations.push(format!(
                        "No structured diagnostics export for {}",
                        u.file.display()
                    ));
                }
                out.execution = Some(e);
            }
            Err(e) => {
                out.failed = true;
                out.limitations.push(format!("{}: {e}", u.file.display()))
            }
        }
        Ok(Outcome::Analyzed(Box::new(out)))
    };
    // Results are merged in unit order so evidence does not depend on scheduling.
    let outcomes = runner::parallel(&units, c.tools.jobs, analyze);
    let mut cancelled = 0;
    // Diagnostics located in an included file are reported by every unit that
    // includes it. Units with the same compile configuration see the same
    // header, so attribute those findings to the first such unit's variant;
    // correlate() then merges them and keeps every unit's native reference.
    let mut representative: HashMap<String, String> = HashMap::new();
    let mut canonical: HashMap<String, Option<PathBuf>> = HashMap::new();
    let mut merged_from = 0;
    for (u, outcome) in units.iter().zip(outcomes) {
        match outcome.transpose()? {
            None => cancelled += 1,
            Some(Outcome::Unsupported) => {
                run.completeness.unsupported += 1;
                run.limitations.extend(
                    u.unsupported
                        .iter()
                        .map(|p| format!("{} {}: {p}", u.file.display(), u.variant)),
                );
            }
            Some(Outcome::Analyzed(out)) => {
                if out.failed {
                    run.completeness.failed += 1
                } else {
                    run.completeness.analyzed += 1
                }
                let config = configuration_key(u);
                let first = representative
                    .entry(config)
                    .or_insert_with(|| u.variant.clone())
                    .clone();
                let source = u.file.canonicalize().ok();
                for mut f in out.findings {
                    let located = canonical
                        .entry(f.file.clone())
                        .or_insert_with(|| Path::new(&f.file).canonicalize().ok());
                    if located.is_some() && *located != source && first != u.variant {
                        f.build_variant = first.clone();
                        merged_from += 1;
                    }
                    run.findings.push(f);
                }
                run.limitations.extend(out.limitations);
                if let Some(e) = out.execution {
                    run.record(e);
                }
            }
        }
    }
    if cancelled > 0 {
        run.completeness.failed += cancelled;
        run.limitations
            .push("Remaining translation units cancelled".into());
    }
    let before = run.findings.len();
    run.findings = findings::correlate(run.findings, root);
    if merged_from > 0 {
        run.limitations.push(format!(
            "{} included-file findings repeated by translation units with identical compile configurations were merged ({} findings before merging, {} after); each merged finding names its first unit's variant and lists every reporting unit log in native_references",
            merged_from,
            before,
            run.findings.len()
        ));
    }
    run.complete = run.completeness.complete();
    if c.coding.c.is_some() || c.coding.cpp.is_some() {
        run.complete = false;
        run.limitations.push("Configured MISRA edition has only partial, independently unreviewed automated mappings and manual guidelines; guideline verification remains incomplete".into());
    }
    run.status = if !run.complete {
        Status::Incomplete
    } else if !run.findings.is_empty() {
        Status::Findings
    } else {
        Status::Passed
    };
    evidence::finish(root, c, &dir, run)
}
/// Compile configuration of a unit: everything except its own source file and
/// output path, so units differing only in which file they compile share it.
fn configuration_key(u: &discovery::Unit) -> String {
    let source = u.file.canonicalize().ok();
    let mut skip_next = false;
    let flags: Vec<&str> = u
        .arguments
        .iter()
        .filter(|a| {
            if std::mem::take(&mut skip_next) {
                return false;
            }
            if a.as_str() == "-o" {
                skip_next = true;
                return false;
            }
            let path = u.directory.join(a.as_str());
            !(a.starts_with("-o") || (source.is_some() && path.canonicalize().ok() == source))
        })
        .map(String::as_str)
        .collect();
    format!(
        "{}\0{}\0{}",
        u.directory.display(),
        u.compiler,
        flags.join("\0")
    )
}
enum Outcome {
    Unsupported,
    Analyzed(Box<Analyzed>),
}
#[derive(Default)]
struct Analyzed {
    failed: bool,
    findings: Vec<crate::model::Finding>,
    limitations: Vec<String>,
    execution: Option<crate::model::Execution>,
}
pub fn profile_check(profile: Option<&str>) -> Result<()> {
    if profile.is_some_and(|p| p != "misra-c-2012" && p != "misra-cpp-2023") {
        bail!("unknown MISRA profile")
    };
    Ok(())
}
