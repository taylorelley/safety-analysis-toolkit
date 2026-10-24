use crate::{
    config::Config,
    discovery, evidence, findings,
    model::{Run, Status},
    runner,
};
use anyhow::{Result, bail};
use std::{fs, path::Path};
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
            let valid = v.failure.is_none() && v.stdout.contains("version 19.1.7");
            run.record(v);
            if !valid {
                run.completeness.unsupported = units.len();
                run.limitations.push("Requires pinned LLVM 19.1.7".into());
                return evidence::finish(root, c, &dir, run);
            }
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
    for (i, u) in units.iter().enumerate() {
        if runner::cancelled() {
            run.completeness.failed += units.len() - i;
            run.limitations
                .push("Remaining translation units cancelled".into());
            break;
        }
        if !u.unsupported.is_empty() {
            run.completeness.unsupported += 1;
            run.limitations.extend(
                u.unsupported
                    .iter()
                    .map(|p| format!("{} {}: {p}", u.file.display(), u.variant)),
            );
            continue;
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
                let mut failed = e.failure.is_some();
                if fixes.exists() {
                    match findings::parse(&fs::read(&fixes)?, &u.variant, &reference, &u.directory)
                    {
                        Ok(f) => {
                            failed |= f.iter().any(|d| d.rule_id == "clang-diagnostic-error");
                            run.findings.extend(f)
                        }
                        Err(err) => {
                            failed = true;
                            run.limitations.push(err.to_string())
                        }
                    }
                } else if !e.stdout.trim().is_empty() || !e.stderr.trim().is_empty() {
                    failed = true;
                    run.limitations.push(format!(
                        "No structured diagnostics export for {}",
                        u.file.display()
                    ));
                }
                if failed {
                    run.completeness.failed += 1
                } else {
                    run.completeness.analyzed += 1
                }
                run.record(e);
            }
            Err(e) => {
                run.completeness.failed += 1;
                run.limitations.push(format!("{}: {e}", u.file.display()))
            }
        }
    }
    run.findings = findings::correlate(run.findings, root);
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
pub fn profile_check(profile: Option<&str>) -> Result<()> {
    if profile.is_some_and(|p| p != "misra-c-2012" && p != "misra-cpp-2023") {
        bail!("unknown MISRA profile")
    };
    Ok(())
}
