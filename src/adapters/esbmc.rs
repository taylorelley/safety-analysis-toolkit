use crate::{
    config::{Config, inside},
    discovery, evidence,
    model::{ProofOutcome, Property, Run, Status},
    runner,
};
use anyhow::{Result, bail};
use std::{fs, path::Path};
fn translate(unit: &discovery::Unit) -> Result<Vec<String>> {
    anyhow::ensure!(
        unit.unsupported.is_empty(),
        "Unreviewed compilation semantics: {:?}",
        unit.unsupported
    );
    let mut out = Vec::new();
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
        if let Some(std) = arg.strip_prefix("-std=") {
            out.extend(["--std".into(), std.into()]);
            continue;
        }
        if arg.starts_with("-D") || arg.starts_with("-U") {
            out.push(arg.clone());
            if arg == "-D" || arg == "-U" {
                out.push(
                    args.next()
                        .ok_or_else(|| anyhow::anyhow!("missing macro value"))?
                        .clone(),
                );
            }
            continue;
        }
        if arg == "-I" {
            out.push("-I".into());
            out.push(
                unit.directory
                    .join(
                        args.next()
                            .ok_or_else(|| anyhow::anyhow!("missing include directory"))?,
                    )
                    .display()
                    .to_string(),
            );
            continue;
        }
        if let Some(path) = arg.strip_prefix("-I") {
            out.push(format!("-I{}", unit.directory.join(path).display()));
            continue;
        }
        bail!("ESBMC semantic translation unavailable for {arg}")
    }
    Ok(out)
}
fn results(log: &str, exit: Option<i32>, bounded: u32, bits: u32, version: &str) -> Vec<Property> {
    let success = exit == Some(0)
        && log.lines().any(|l| l == "VERIFICATION SUCCESSFUL")
        && !log.to_lowercase().contains("warning");
    let failed = log.lines().any(|l| l == "VERIFICATION FAILED");
    let mut properties = Vec::new();
    for line in log.lines() {
        let line = line.trim();
        if !(line.starts_with("PASSED ") || line.starts_with("FAILED ")) {
            continue;
        }
        let Some((_, rest)) = line.split_once('[') else {
            continue;
        };
        let Some((id, description)) = rest.split_once(']') else {
            continue;
        };
        let unwind = description.to_lowercase().contains("unwinding assertion");
        let outcome = if line.starts_with("FAILED ") && failed && !unwind {
            ProofOutcome::Violated
        } else if line.starts_with("PASSED ") && success {
            ProofOutcome::Proven
        } else {
            ProofOutcome::Unknown
        };
        properties.push(Property{id:id.into(),description:description.trim().into(),outcome,scope:format!("ESBMC {version} sequential source model with entry main; {bits}-bit little-endian; Z3; finite unwind {bounded}; default unwinding assertions enabled; source assumptions and library models apply"),assumptions:Vec::new(),unwind_bound:Some(bounded),completeness_established:success,native_references:vec!["raw/esbmc.log".into()]});
    }
    if properties.is_empty() {
        properties.push(Property {
            id: "analysis".into(),
            description: "No individually classified ESBMC properties".into(),
            outcome: ProofOutcome::Unknown,
            scope: "Property enumeration unavailable".into(),
            assumptions: Vec::new(),
            unwind_bound: Some(bounded),
            completeness_established: false,
            native_references: vec!["raw/esbmc.log".into()],
        });
    }
    properties
}
/// Limitation when ESBMC stopped at the unwinding bound: loops were not fully
/// explored, so affected properties stay unknown rather than proven/violated.
fn unwind_hint(properties: &[Property], unwind: u32) -> Option<String> {
    properties
        .iter()
        .any(|p| {
            p.outcome != ProofOutcome::Proven
                && p.description.to_lowercase().contains("unwinding assertion")
        })
        .then(|| {
            format!(
                "ESBMC reached the unwinding bound {unwind} before exploring all loop iterations; results are unknown, not proven or violated. Re-run with a larger --unwind"
            )
        })
}
pub struct Options<'a> {
    pub unwind: u32,
    pub bits: u32,
    pub method: &'a str,
    pub entry: &'a str,
}
pub fn prove(
    root: &Path,
    c: &Config,
    source: &Path,
    variant: Option<&str>,
    options: Options<'_>,
) -> Result<Run> {
    let Options {
        unwind,
        bits,
        method,
        entry,
    } = options;
    anyhow::ensure!(
        ["bmc", "induction"].contains(&method),
        "unsupported ESBMC method"
    );

    anyhow::ensure!(
        (1..=10000).contains(&unwind) && [32, 64].contains(&bits),
        "invalid verification bounds or target bits"
    );
    let source = inside(root, source)?;
    let units = discovery::database(root, &c.build.compile_commands)?;
    let candidates: Vec<_> = units
        .iter()
        .filter(|u| u.file == source && variant.is_none_or(|v| v == u.variant))
        .collect();
    anyhow::ensure!(
        candidates.len() == 1,
        "select exactly one compilation variant with --variant; source must be in compilation database"
    );
    let unit = candidates[0];
    let (mut run, dir) = evidence::begin(root, c, "prove")?;
    run.completeness.total = 1;
    run.completeness.build_variants = vec![unit.variant.clone()];
    let Some(tool) = discovery::executable("esbmc", &c.tools.allowlist) else {
        run.completeness.failed = 1;
        run.limitations
            .push("ESBMC unavailable or not allowlisted; properties NOT_CHECKED".into());
        return evidence::finish(root, c, &dir, run);
    };
    let extra = match translate(unit) {
        Ok(a) => a,
        Err(e) => {
            run.completeness.unsupported = 1;
            run.limitations.push(e.to_string());
            return evidence::finish(root, c, &dir, run);
        }
    };
    let version = match runner::execute(
        &tool,
        &["--version".into()],
        root,
        &dir,
        c.tools.timeout_seconds,
        c.tools.memory_mb,
        c.tools.require_filesystem_isolation,
    ) {
        Ok(e) => e,
        Err(e) => {
            run.completeness.failed = 1;
            run.limitations.push(e.to_string());
            return evidence::finish(root, c, &dir, run);
        }
    };
    let detected = discovery::version_after(&version.stdout, "ESBMC version ")
        .filter(|v| version.failure.is_none() && discovery::at_least(v, &discovery::ESBMC_MINIMUM));
    run.record(version);
    let Some(detected) = detected else {
        run.completeness.unsupported = 1;
        run.limitations.push("Requires ESBMC 8.6.0 or later".into());
        return evidence::finish(root, c, &dir, run);
    };
    let detected = detected
        .iter()
        .map(u32::to_string)
        .collect::<Vec<_>>()
        .join(".");
    run.limitations.extend(discovery::reference_note(
        "ESBMC",
        &detected,
        discovery::ESBMC_REFERENCE,
    ));
    let mut args = std::iter::once(source.display().to_string())
        .chain(vec![
            "--z3".into(),
            "--unwind".into(),
            unwind.to_string(),
            format!("--{bits}"),
            "--overflow-check".into(),
            "--generate-json-report".into(),
        ])
        .chain(extra)
        .collect::<Vec<_>>();
    args.extend(["--function".into(), entry.into()]);
    if method == "induction" {
        args.extend([
            "--k-induction".into(),
            "--max-k-step".into(),
            unwind.to_string(),
        ]);
    }
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
            evidence::write_new(&dir.join("raw/esbmc.log"), log.as_bytes())?;
            run.properties = results(&log, e.exit_code, unwind, bits, &detected);
            run.limitations.extend(unwind_hint(&run.properties, unwind));
            for p in &mut run.properties {
                p.scope = p.scope.replace("entry main", &format!("entry {entry}"));
                if method == "induction" {
                    p.scope.push_str("; k-induction requested; phase logs preserved; finite bound is the induction search limit, not an unrestricted model claim");
                }
            }

            if let Some(failure) = &e.failure
                && failure != "nonzero_exit"
            {
                for p in &mut run.properties {
                    p.outcome = ProofOutcome::Failed;
                    p.completeness_established = false;
                }
            }
            if dir.join("report.json").exists() {
                let raw = fs::read(dir.join("report.json"))?;
                let parsed: serde_json::Value = serde_json::from_slice(&raw)?;
                evidence::write_new(
                    &dir.join("counterexamples.json"),
                    &serde_json::to_vec_pretty(&parsed)?,
                )?;
            }
            run.complete = run
                .properties
                .iter()
                .all(|p| matches!(p.outcome, ProofOutcome::Proven | ProofOutcome::Violated));
            if run.complete {
                run.completeness.analyzed = 1
            } else {
                run.completeness.failed = 1
            }
            run.status = if !run.complete {
                Status::Incomplete
            } else if run
                .properties
                .iter()
                .any(|p| p.outcome == ProofOutcome::Violated)
            {
                Status::Findings
            } else {
                Status::Passed
            };
            run.record(e);
        }
        Err(e) => {
            run.completeness.failed = 1;
            run.limitations.push(e.to_string());
        }
    }
    run.limitations.push("Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.".into());
    evidence::finish(root, c, &dir, run)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn unwind_bound_hint() {
        let stopped = results(
            "FAILED [P1] unwinding assertion loop 3\nVERIFICATION FAILED",
            Some(1),
            8,
            64,
            "8.6.0",
        );
        assert!(unwind_hint(&stopped, 8).unwrap().contains("--unwind"));
        let complete = results(
            "PASSED [P1] unwinding assertion loop 3\nVERIFICATION SUCCESSFUL",
            Some(0),
            12,
            64,
            "8.6.0",
        );
        assert_eq!(unwind_hint(&complete, 12), None);
    }
    #[test]
    fn no_false_proof() {
        for (log, exit) in [
            ("VERIFICATION SUCCESSFUL", Some(1)),
            (
                "PASSED [P] x\nVERIFICATION SUCCESSFUL\nwarning: unsupported",
                Some(0),
            ),
            (
                "FAILED [P] unwinding assertion\nVERIFICATION FAILED",
                Some(10),
            ),
        ] {
            assert!(
                results(log, exit, 8, 64, "8.6.0")
                    .iter()
                    .all(|p| p.outcome != ProofOutcome::Proven)
            );
        }
    }
}
