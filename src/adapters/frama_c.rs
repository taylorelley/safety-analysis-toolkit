use crate::{
    config::{Config, inside},
    discovery, evidence,
    model::{ProofOutcome, Property, Run, Status},
    runner,
};
use anyhow::{Context, Result, bail};
use std::{fs, path::Path};

fn preprocess_options(unit: &discovery::Unit) -> Result<Vec<String>> {
    anyhow::ensure!(
        unit.unsupported.is_empty(),
        "Unreviewed compilation semantics: {:?}",
        unit.unsupported
    );
    let mut result = Vec::new();
    let mut args = unit.arguments.iter();
    while let Some(arg) = args.next() {
        if arg == "-c" {
            continue;
        }
        if arg == "-o" {
            args.next();
            continue;
        }
        if !arg.starts_with('-')
            && unit.directory.join(arg).canonicalize().ok().as_ref() == Some(&unit.file)
        {
            continue;
        }
        if arg.starts_with("-std=") {
            result.push(arg.clone());
            continue;
        }
        if [
            "-D", "-U", "-I", "-isystem", "-iquote", "-include", "-imacros",
        ]
        .contains(&arg.as_str())
        {
            let value = args.next().context("missing compiler option value")?;
            result.push(arg.clone());
            result.push(if arg == "-D" || arg == "-U" {
                value.clone()
            } else {
                unit.directory.join(value).display().to_string()
            });
            continue;
        }
        if arg.starts_with("-D") || arg.starts_with("-U") {
            result.push(arg.clone());
            continue;
        }
        if let Some(value) = arg.strip_prefix("-I") {
            result.push(format!("-I{}", unit.directory.join(value).display()));
            continue;
        }
        bail!("Frama-C semantic translation unavailable for {arg}")
    }
    Ok(result)
}
fn incomplete(
    mut run: Run,
    root: &Path,
    c: &Config,
    dir: &Path,
    reason: String,
    failed: bool,
) -> Result<Run> {
    if failed {
        run.completeness.failed = 1
    } else {
        run.completeness.unsupported = 1
    }
    run.limitations.push(reason.clone());
    run.properties.push(Property {
        id: "frama-c-analysis".into(),
        description: reason,
        outcome: if failed {
            ProofOutcome::Failed
        } else {
            ProofOutcome::NotChecked
        },
        scope: "No completed Frama-C property analysis".into(),
        assumptions: Vec::new(),
        unwind_bound: None,
        completeness_established: false,
        native_references: vec!["raw/frama-c.log".into()],
    });
    evidence::finish(root, c, dir, run)
}
// Version-specific statuses are interpreted conservatively. In particular
// 'Considered valid' and 'Invalid or unreachable' are never proof success.
fn properties(
    table: &[u8],
    cwd: &Path,
    root: &Path,
    method: &str,
    entry: &str,
    bits: u32,
    clean: bool,
) -> Result<Vec<Property>> {
    let mut reader = csv::ReaderBuilder::new()
        .delimiter(b'\t')
        .from_reader(table);
    anyhow::ensure!(
        reader.headers()?.iter().eq([
            "directory",
            "file",
            "line",
            "function",
            "property kind",
            "status",
            "property"
        ]),
        "unsupported Frama-C report columns"
    );
    let mut assumptions = Vec::new();
    let mut selected = Vec::new();
    for row in reader.records() {
        let row = row?;
        let location = cwd.join(&row[0]).join(&row[1]);
        let in_project = location.canonicalize().is_ok_and(|p| p.starts_with(root));
        let kind = &row[4];
        let status = &row[5];
        let description = format!(
            "{}:{} {} {}: {}",
            location.display(),
            &row[2],
            &row[3],
            kind,
            &row[6]
        );
        if !in_project || status == "Considered valid" || (method == "wp" && kind == "precondition")
        {
            assumptions.push(format!("Native model premise ({status}): {description}"));
            continue;
        }
        selected.push((description, status.to_string()));
    }
    let no_unknown = selected.iter().all(|(_, s)| s == "Valid");
    let mut result = Vec::new();
    for (description, status) in selected {
        let outcome = if clean && status == "Valid" && no_unknown {
            ProofOutcome::Proven
        } else {
            ProofOutcome::Unknown
        };
        result.push(Property {
            id: format!("FC-{}", &evidence::hash(description.as_bytes())[..20]),
            description: format!("{description} [native status: {status}]"),
            outcome,
            scope: format!(
                "{method}, entry {entry}, x86 {bits}-bit model; {}",
                if method == "eva" {
                    "reachable sequential C states; alarms and native assumptions apply"
                } else {
                    "supplied ACSL contracts; WP typed memory model, RTE obligations and Qed only"
                }
            ),
            assumptions: assumptions.clone(),
            unwind_bound: None,
            completeness_established: clean && no_unknown,
            native_references: vec![
                "raw/frama-c.log".into(),
                "raw/frama-c-properties.tsv".into(),
            ],
        });
    }
    if result.is_empty() {
        result.push(Property {
            id: "frama-c-no-properties".into(),
            description: "No selected project property was enumerated".into(),
            outcome: ProofOutcome::NotChecked,
            scope: format!("{method} entry {entry}; no vacuous proof claim"),
            assumptions,
            unwind_bound: None,
            completeness_established: false,
            native_references: vec!["raw/frama-c.log".into()],
        });
    }
    Ok(result)
}
pub fn prove(
    root: &Path,
    c: &Config,
    source: &Path,
    variant: Option<&str>,
    method: &str,
    entry: &str,
    bits: u32,
) -> Result<Run> {
    anyhow::ensure!(
        ["eva", "wp"].contains(&method)
            && [32, 64].contains(&bits)
            && !entry.is_empty()
            && entry.chars().all(|c| c.is_ascii_alphanumeric() || c == '_'),
        "invalid Frama-C mode/entry/target model"
    );
    let source = inside(root, source)?;
    let (mut run, dir) = evidence::begin(root, c, "prove")?;
    run.completeness.total = 1;
    if source.extension().is_none_or(|x| x != "c") {
        return incomplete(run, root, c, &dir, "Frama-C supports C only".into(), false);
    }
    let units = discovery::database(root, &c.build.compile_commands)?;
    let selected: Vec<_> = units
        .iter()
        .filter(|u| u.file == source && variant.is_none_or(|v| v == u.variant))
        .collect();
    if selected.len() != 1 {
        return incomplete(
            run,
            root,
            c,
            &dir,
            "Select exactly one Frama-C compilation variant".into(),
            false,
        );
    }
    let unit = selected[0];
    run.completeness.build_variants = vec![unit.variant.clone()];
    let extra = match preprocess_options(unit) {
        Ok(x) => x,
        Err(e) => return incomplete(run, root, c, &dir, e.to_string(), false),
    };
    let Some(tool) = discovery::executable("frama-c", &c.tools.allowlist) else {
        return incomplete(
            run,
            root,
            c,
            &dir,
            "Frama-C unavailable or not allowlisted".into(),
            false,
        );
    };
    let Some(clang) = discovery::executable("clang-19", &c.tools.allowlist) else {
        return incomplete(
            run,
            root,
            c,
            &dir,
            "Clang 19 preprocessor unavailable or not allowlisted".into(),
            false,
        );
    };
    let mut version = String::new();
    for args in [vec!["-version".into()], vec!["-print-share-path".into()]] {
        match runner::execute(
            &tool,
            &args,
            root,
            &dir,
            c.tools.timeout_seconds,
            c.tools.memory_mb,
            c.tools.require_filesystem_isolation,
        ) {
            Ok(e) => {
                let failed = e.failure.is_some();
                // A recognised release below the minimum is unsupported, not a
                // failure; an unrecognised version or failed call is a failure.
                let recognised = discovery::version_after(e.stdout.trim(), "");
                if args[0] == "-version" {
                    version = e.stdout.trim().to_string();
                }
                run.record(e);
                if failed {
                    return incomplete(
                        run,
                        root,
                        c,
                        &dir,
                        "Frama-C capability failure".into(),
                        true,
                    );
                }
                if args[0] == "-version" {
                    match recognised {
                        Some(v) if discovery::at_least(&v, &discovery::FRAMA_C_MINIMUM) => {
                            run.limitations.extend(discovery::reference_note(
                                "Frama-C",
                                &version,
                                discovery::FRAMA_C_REFERENCE,
                            ))
                        }
                        Some(_) => {
                            return incomplete(
                                run,
                                root,
                                c,
                                &dir,
                                "Unsupported Frama-C version; requires Frama-C 25.0 or later"
                                    .into(),
                                false,
                            );
                        }
                        None => {
                            return incomplete(
                                run,
                                root,
                                c,
                                &dir,
                                "Unrecognised Frama-C version; requires Frama-C 25.0 or later"
                                    .into(),
                                true,
                            );
                        }
                    }
                }
            }
            Err(e) => return incomplete(run, root, c, &dir, e.to_string(), true),
        }
    }
    let share = std::path::PathBuf::from(run.executions.last().unwrap().stdout.trim());
    let libc = share.join("libc");
    if !libc.is_dir() {
        return incomplete(
            run,
            root,
            c,
            &dir,
            "Frama-C libc model directory unavailable".into(),
            true,
        );
    }
    let input = dir.join("analysis.i");
    let mut args = vec![
        "-E".into(),
        "-CC".into(),
        "-nostdinc".into(),
        format!("-I{}", libc.display()),
        "-D__FRAMAC__".into(),
        format!("-D__FC_MACHDEP_X86_{}", bits),
        format!("-m{bits}"),
        "-MD".into(),
        "-MF".into(),
        dir.join("dependencies.d").display().to_string(),
    ];
    args.extend(extra);
    args.extend([
        source.display().to_string(),
        "-o".into(),
        input.display().to_string(),
    ]);
    match runner::execute(
        &clang,
        &args,
        &unit.directory,
        &dir,
        c.tools.timeout_seconds,
        c.tools.memory_mb,
        c.tools.require_filesystem_isolation,
    ) {
        Ok(e) => {
            let good = e.failure.is_none() && e.stderr.trim().is_empty();
            evidence::write_new(
                &dir.join("raw/preprocessor.log"),
                format!("{}\n{}", e.stdout, e.stderr).as_bytes(),
            )?;
            run.record(e);
            if !good {
                return incomplete(
                    run,
                    root,
                    c,
                    &dir,
                    "C preprocessing failed or emitted warnings".into(),
                    true,
                );
            }
        }
        Err(e) => return incomplete(run, root, c, &dir, e.to_string(), true),
    }
    let report = dir.join("raw/frama-c-properties.tsv");
    let mut args = vec![
        "-machdep".into(),
        format!("x86_{bits}"),
        "-main".into(),
        entry.into(),
        input.display().to_string(),
    ];
    if method == "eva" {
        args.extend(["-eva".into(), "-eva-no-reduce-on-logic-alarms".into()]);
    } else {
        args.extend([
            "-wp".into(),
            "-wp-fct".into(),
            entry.into(),
            "-wp-prover".into(),
            "qed".into(),
            "-wp-rte".into(),
            "-wp-report-json".into(),
            dir.join("raw/wp.json").display().to_string(),
        ]);
    }
    args.extend([
        "-then".into(),
        "-report".into(),
        "-report-proven".into(),
        "-report-untried".into(),
        "-report-csv".into(),
        report.display().to_string(),
    ]);
    match runner::execute(
        &tool,
        &args,
        &dir,
        &dir,
        c.tools.timeout_seconds,
        c.tools.memory_mb,
        c.tools.require_filesystem_isolation,
    ) {
        Ok(e) => {
            let log = format!("{}\n{}", e.stdout, e.stderr);
            evidence::write_new(&dir.join("raw/frama-c.log"), log.as_bytes())?;
            let clean = e.failure.is_none()
                && (if method == "eva" {
                    log.contains("====== ANALYSIS SUMMARY ======")
                } else {
                    log.contains("[wp] Proved goals:")
                })
                && !log.contains("Neither code nor specification")
                && !log.to_lowercase().contains("unsupported")
                && !log.to_lowercase().contains("warning:")
                && !log.contains("User Error");
            run.record(e);
            if !report.exists() {
                return incomplete(
                    run,
                    root,
                    c,
                    &dir,
                    "Frama-C produced no structured property report".into(),
                    true,
                );
            }
            run.properties =
                match properties(&fs::read(&report)?, &dir, root, method, entry, bits, clean) {
                    Ok(p) => p,
                    Err(e) => return incomplete(run, root, c, &dir, e.to_string(), true),
                };
            for p in &mut run.properties {
                p.scope = format!("Frama-C {version}, {}", p.scope);
            }
            if run.executions.last().is_some_and(|e| e.failure.is_some()) {
                for p in &mut run.properties {
                    p.outcome = ProofOutcome::Failed;
                    p.completeness_established = false;
                }
            }
        }
        Err(e) => return incomplete(run, root, c, &dir, e.to_string(), true),
    }
    run.complete = run
        .properties
        .iter()
        .all(|p| p.outcome == ProofOutcome::Proven);
    if run.complete {
        run.completeness.analyzed = 1;
        run.status = Status::Passed
    } else {
        run.completeness.failed = 1;
        run.status = Status::Incomplete
    }
    run.limitations.push("Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.".into());
    evidence::finish(root, c, &dir, run)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn external_assumptions_are_not_proofs() {
        let t = tempfile::tempdir().unwrap();
        let data=b"directory\tfile\tline\tfunction\tproperty kind\tstatus\tproperty\n/none\tx.c\t1\tf\tassertion\tConsidered valid\tx\n";
        let p = properties(data, t.path(), t.path(), "eva", "main", 64, true).unwrap();
        assert_eq!(p[0].outcome, ProofOutcome::NotChecked);
    }
}
