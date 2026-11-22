mod esbmc;
mod lean;
mod verus;

use crate::{
    config::{Config, inside, read_bounded, relative},
    evidence,
    model::{Finding, ProofOutcome, Property, Run, Status},
};
use anyhow::{Context, Result, ensure};
use serde::Deserialize;
use serde_json::{Value, json};
use std::{
    collections::{BTreeMap, BTreeSet},
    path::Path,
};

const PLAN: &str = "rust-verification.json";
const MAX_IMPORT: u64 = 16 * 1024 * 1024;

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Plan {
    version: u32,
    invocations: BTreeMap<String, Expected>,
}

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Expected {
    variant: String,
    toolchain: BTreeMap<String, String>,
    command: Vec<String>,
    checks: Checks,
}

#[derive(Deserialize)]
#[serde(tag = "format", rename_all = "snake_case", deny_unknown_fields)]
enum Checks {
    Clippy {
        package: String,
        target: String,
        kind: String,
    },
    Verus(verus::Spec),
    EsbmcGoto(esbmc::Spec),
    AeneasLean(lean::Spec),
    KaniCbmc {
        harness: String,
        unwind: u32,
        completeness: BTreeSet<String>,
        safety_checks: BTreeSet<String>,
        obligations: BTreeMap<String, Obligation>,
    },
}

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Obligation {
    requirement: String,
    proposition: String,
    native_check: String,
    reachability: BTreeSet<String>,
    assumptions: Vec<String>,
}

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Bundle {
    version: u32,
    producer: String,
    inputs: BTreeMap<String, String>,
    invocations: BTreeMap<String, Invocation>,
}

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Invocation {
    toolchain: BTreeMap<String, String>,
    command: Vec<String>,
    exit_code: i32,
    output_sha256: String,
    output: String,
    #[serde(default)]
    diagnostics: Option<String>,
    #[serde(default)]
    regenerated_models: Option<BTreeMap<String, String>>,
}

struct Attempt<'a> {
    id: &'a str,
    variant: &'a str,
    raw_ref: &'a str,
    actual: Option<&'a Invocation>,
}

fn mapped(result: &Result<ProofOutcome>, actual: Option<&Invocation>) -> ProofOutcome {
    result.as_ref().cloned().unwrap_or(if actual.is_some() {
        ProofOutcome::Failed
    } else {
        ProofOutcome::NotChecked
    })
}

fn planned(root: &Path) -> Result<(Vec<u8>, Plan)> {
    let bytes = read_bounded(&inside(root, Path::new(PLAN))?, 1024 * 1024)?;
    let plan: Plan = serde_json::from_slice(&bytes)?;
    ensure!(
        plan.version == 1,
        "unsupported Rust verification plan version"
    );
    ensure!(!plan.invocations.is_empty(), "no expected Rust invocations");
    ensure!(plan.invocations.len() <= 100, "too many Rust invocations");
    let mut obligation_ids = BTreeSet::new();
    let mut replays = BTreeSet::new();
    for (id, expected) in &plan.invocations {
        ensure!(
            label(id) && label(&expected.variant),
            "invalid invocation or variant ID"
        );
        ensure!(
            !expected.command.is_empty(),
            "missing reviewed command for {id}"
        );
        ensure!(
            [
                "rustc",
                "cargo",
                match expected.checks {
                    Checks::Clippy { .. } => "clippy",
                    Checks::Verus(_) => "verus",
                    Checks::EsbmcGoto(_) => "esbmc",
                    Checks::AeneasLean(_) => "lean",
                    Checks::KaniCbmc { .. } => "kani",
                }
            ]
            .iter()
            .all(|key| expected
                .toolchain
                .get(*key)
                .is_some_and(|v| !v.trim().is_empty())),
            "missing reviewed toolchain versions for {id}"
        );
        if let Checks::Verus(spec) = &expected.checks {
            ensure!(
                obligation_ids.insert((id, &expected.variant)),
                "repeated obligation {id}"
            );
            spec.validate(expected)?;
        }
        if let Checks::AeneasLean(spec) = &expected.checks {
            ensure!(
                obligation_ids.insert((id, &expected.variant)),
                "repeated obligation {id}"
            );
            spec.validate(expected)?;
        }
        if let Checks::EsbmcGoto(spec) = &expected.checks {
            ensure!(
                replays.insert(&spec.cbmc),
                "repeated ESBMC replay for {}",
                spec.cbmc
            );
            spec.validate(expected, &plan)?;
        }
        if let Checks::KaniCbmc {
            harness,
            unwind,
            completeness,
            safety_checks,
            obligations,
        } = &expected.checks
        {
            ensure!(
                !harness.trim().is_empty() && *unwind > 0,
                "missing proof scope for {id}"
            );
            ensure!(
                expected
                    .toolchain
                    .get("cbmc")
                    .is_some_and(|v| !v.trim().is_empty()),
                "missing CBMC version for {id}"
            );
            ensure!(!obligations.is_empty(), "no proof obligations for {id}");
            let mut native = BTreeSet::new();
            for check in completeness.iter().chain(safety_checks).chain(
                obligations
                    .values()
                    .flat_map(|o| std::iter::once(&o.native_check).chain(&o.reachability)),
            ) {
                ensure!(
                    !check.is_empty() && native.insert(check),
                    "empty or repeated native check in {id}"
                );
            }
            for (key, obligation) in obligations {
                ensure!(
                    label(key)
                        && obligation_ids.insert((key, &expected.variant))
                        && !obligation.requirement.trim().is_empty()
                        && !obligation.proposition.trim().is_empty()
                        && !obligation.reachability.is_empty(),
                    "invalid or repeated obligation {key} in {id}"
                );
            }
        }
    }
    Ok((bytes, plan))
}

fn label(s: &str) -> bool {
    !s.is_empty()
        && s.len() <= 128
        && s.bytes()
            .all(|b| b.is_ascii_alphanumeric() || b"-_.".contains(&b))
}

pub fn snapshot(root: &Path, c: &Config) -> Result<BTreeMap<String, String>> {
    evidence::fingerprint(root, &root.join(&c.evidence.directory))
}

pub fn import(root: &Path, c: &Config, path: &Path) -> Result<Run> {
    relative(path)?;
    let path = inside(root, path)?;
    let bundle_rel = path
        .strip_prefix(root)?
        .to_str()
        .context("non-UTF-8 import path")?;
    ensure!(
        !path.starts_with(root.join(&c.evidence.directory))
            && bundle_rel != PLAN
            && bundle_rel != "safety.yaml",
        "Rust bundle must be a separate workspace file outside the evidence directory"
    );
    let (plan_bytes, plan) = planned(root)?;
    let bytes = read_bounded(&path, MAX_IMPORT)?;
    let bundle: Bundle = serde_json::from_slice(&bytes)?;
    ensure!(
        bundle.version == 1 && !bundle.producer.trim().is_empty(),
        "invalid Rust bundle header"
    );
    let (mut run, dir) = evidence::begin(root, c, "rust-import")?;
    evidence::write_new(&dir.join("rust-verification.json"), &plan_bytes)?;
    evidence::write_new(&dir.join("rust-import.json"), &bytes)?;
    run.completeness.total = plan.invocations.len();
    run.completeness.build_variants = plan
        .invocations
        .values()
        .map(|entry| entry.variant.clone())
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect();
    run.verification = json!({"origin":"supplied_ci_record","producer":bundle.producer,"plan":"rust-verification.json","bundle":"rust-import.json"});
    run.limitations.push("Rust verifier execution and toolchain identity are supplied assertions. The toolkit checks consistency and records results within the declared model.".into());

    let mut expected_inputs = run.inputs.clone();
    let bundle_hash = expected_inputs.remove(bundle_rel);
    let trusted = expected_inputs == bundle.inputs
        && bundle_hash.as_deref() == Some(evidence::hash(&bytes).as_str())
        && expected_inputs.get(PLAN) == Some(&evidence::hash(&plan_bytes));
    if !trusted {
        run.limitations
            .push("Rust bundle does not match the complete workspace input inventory".into());
    }
    let extra = bundle
        .invocations
        .keys()
        .any(|id| !plan.invocations.contains_key(id));
    if extra {
        run.limitations
            .push("Rust bundle contains unplanned invocations".into());
    }
    let mut witnesses = Vec::new();
    let mut completed = BTreeMap::<&str, std::ops::Range<usize>>::new();
    let invocations = plan.invocations.iter().enumerate();
    let is_replay =
        |entry: &(usize, (&String, &Expected))| matches!(entry.1.1.checks, Checks::EsbmcGoto(_));
    for (index, (id, expected)) in invocations
        .clone()
        .filter(|entry| !is_replay(entry))
        .chain(invocations.filter(is_replay))
    {
        let mut raw_ref = "rust-import.json".to_string();
        let mut record = None;
        if let Some(actual) = bundle.invocations.get(id) {
            raw_ref = format!("raw/rust-{index}.txt");
            evidence::write_new(&dir.join(&raw_ref), actual.output.as_bytes())?;
            if let Some(diagnostics) = &actual.diagnostics {
                evidence::write_new(
                    &dir.join(format!("{raw_ref}.stderr")),
                    diagnostics.as_bytes(),
                )?;
            }
            if trusted
                && actual.command == expected.command
                && actual.toolchain == expected.toolchain
                && actual.output_sha256 == evidence::hash(actual.output.as_bytes())
            {
                record = Some(actual);
            }
        }
        let start = run.properties.len();
        let attempt = Attempt {
            id,
            variant: &expected.variant,
            raw_ref: &raw_ref,
            actual: record,
        };
        let result = match &expected.checks {
            Checks::Clippy {
                package,
                target,
                kind,
            } => record
                .context("missing invocation or inconsistent provenance")
                .and_then(|actual| {
                    clippy(&mut run, expected, package, target, kind, actual, &raw_ref)
                }),
            Checks::Verus(spec) => spec.record(&mut run, attempt),
            Checks::AeneasLean(spec) => spec.record(&mut run, attempt),
            Checks::EsbmcGoto(spec) => {
                let paired = completed
                    .get(spec.cbmc.as_str())
                    .map(|range| run.properties[range.clone()].to_vec());
                spec.record(&mut run, attempt, paired.as_deref(), &mut witnesses)
            }
            Checks::KaniCbmc { .. } => proof(&mut run, expected, record, &raw_ref, &mut witnesses),
        };
        match result {
            Ok(()) => {
                run.completeness.analyzed += 1;
                if matches!(expected.checks, Checks::KaniCbmc { .. }) {
                    completed.insert(id, start..run.properties.len());
                }
            }
            Err(e) => {
                run.completeness.failed += 1;
                run.limitations.push(format!("Rust invocation {id}: {e}"));
            }
        }
    }
    if !witnesses.is_empty() {
        evidence::write_new(
            &dir.join("counterexamples.json"),
            &serde_json::to_vec(&witnesses)?,
        )?;
    }
    for (index, f) in run.findings.iter_mut().enumerate() {
        f.id = format!("rust-{}", index + 1);
    }
    run.complete = !extra && run.completeness.complete();
    run.status = if !run.complete {
        Status::Incomplete
    } else if !run.findings.is_empty()
        || run
            .properties
            .iter()
            .any(|p| p.outcome == ProofOutcome::Violated)
    {
        Status::Findings
    } else {
        Status::Passed
    };
    evidence::finish(root, c, &dir, run)
}

fn clippy(
    run: &mut Run,
    expected: &Expected,
    package: &str,
    target: &str,
    kind: &str,
    actual: &Invocation,
    raw_ref: &str,
) -> Result<()> {
    let mut built = false;
    let mut finished = false;
    let mut compile_error = false;
    for line in actual.output.lines() {
        ensure!(!finished, "trailing records after Cargo build-finished");
        let value: Value = serde_json::from_str(line).context("malformed Cargo diagnostic JSON")?;
        let same_target = value["package_id"] == package
            && value["target"]["name"] == target
            && value["target"]["kind"]
                .as_array()
                .is_some_and(|kinds| kinds.iter().any(|k| k == kind));
        match value["reason"].as_str() {
            Some("compiler-artifact") if same_target && value["fresh"] == false => built = true,
            Some("compiler-message") => {
                let msg = &value["message"];
                let level = msg["level"].as_str().context("missing diagnostic level")?;
                compile_error |= level == "error" || level == "failure-note";
                if !same_target || !matches!(level, "warning" | "error") {
                    continue;
                }
                let span = msg["spans"]
                    .as_array()
                    .and_then(|spans| spans.iter().find(|s| s["is_primary"] == true));
                let file = span
                    .and_then(|s| s["file_name"].as_str())
                    .unwrap_or_default();
                if !run.inputs.contains_key(file) {
                    run.limitations
                        .push(format!("Rust diagnostic outside recorded inputs: {file}"));
                    continue;
                }
                let span = span.context("missing primary span")?;
                run.findings.push(Finding {
                    tool: "clippy".into(),
                    rule_id: msg["code"]["code"]
                        .as_str()
                        .unwrap_or("rustc::diagnostic")
                        .into(),
                    message: msg["message"]
                        .as_str()
                        .context("missing diagnostic message")?
                        .into(),
                    file: file.into(),
                    line: span["line_start"].as_u64().unwrap_or(1) as usize,
                    column: span["column_start"].as_u64().unwrap_or(1) as usize,
                    severity: level.into(),
                    build_variant: expected.variant.clone(),
                    outcome: "DIAGNOSTIC".into(),
                    native_references: vec![raw_ref.into()],
                    ..Finding::default()
                });
            }
            Some("build-finished") => {
                finished = true;
                compile_error |= value["success"] != true;
            }
            Some(_) => {}
            None => anyhow::bail!("missing Cargo message reason"),
        }
    }
    ensure!(
        finished && built && !compile_error && actual.exit_code == 0,
        "Cargo invocation did not establish fresh successful analysis for the reviewed target"
    );
    Ok(())
}

#[derive(Deserialize)]
struct NativeCheck {
    property: String,
    status: String,
    #[serde(default)]
    trace: Vec<Value>,
}

fn proof(
    run: &mut Run,
    expected: &Expected,
    actual: Option<&Invocation>,
    raw_ref: &str,
    witnesses: &mut Vec<Value>,
) -> Result<()> {
    let Checks::KaniCbmc {
        harness,
        unwind,
        completeness,
        safety_checks,
        obligations,
    } = &expected.checks
    else {
        anyhow::bail!("expected a proof invocation");
    };
    let parsed = actual
        .context("missing invocation or inconsistent provenance")
        .and_then(|actual| {
            ensure!(
                matches!(actual.exit_code, 0 | 10),
                "unexpected CBMC exit code"
            );
            let records: Vec<Value> =
                serde_json::from_str(&actual.output).context("malformed CBMC JSON")?;
            ensure!(
                records.last().and_then(|r| r["cProverStatus"].as_str())
                    == Some(if actual.exit_code == 0 {
                        "success"
                    } else {
                        "failure"
                    }),
                "missing or inconsistent CBMC terminal status"
            );
            ensure!(
                !records.iter().any(|r| r["messageType"] == "ERROR"),
                "CBMC reported an error"
            );
            let mut checks = BTreeMap::new();
            let mut result_frames = 0;
            for result in records.iter().filter_map(|r| r.get("result")) {
                result_frames += 1;
                for check in serde_json::from_value::<Vec<NativeCheck>>(result.clone())? {
                    ensure!(
                        checks.insert(check.property.clone(), check).is_none(),
                        "repeated CBMC property"
                    );
                }
            }
            ensure!(result_frames == 1, "expected one CBMC result frame");
            let ids = completeness
                .iter()
                .chain(safety_checks)
                .chain(
                    obligations
                        .values()
                        .flat_map(|o| std::iter::once(&o.native_check).chain(&o.reachability)),
                )
                .collect::<BTreeSet<_>>();
            ensure!(
                checks.keys().collect::<BTreeSet<_>>() == ids,
                "CBMC checks differ from reviewed inventory"
            );
            Ok(checks)
        });

    let mut conclusive = true;
    for (id, obligation) in obligations {
        let mut references = vec![raw_ref.to_string()];
        let mut model_complete = false;
        let outcome = match &parsed {
            Ok(checks) => {
                let check = &checks[&obligation.native_check];
                references.push(format!("{raw_ref}#property={}", check.property));
                let reachable = obligation
                    .reachability
                    .iter()
                    .all(|id| checks[id].status == "FAILURE" && !checks[id].trace.is_empty());
                model_complete = reachable
                    && completeness
                        .iter()
                        .chain(safety_checks)
                        .all(|id| checks[id].status == "SUCCESS");
                match check.status.as_str() {
                    "SUCCESS" if model_complete => ProofOutcome::Proven,
                    "FAILURE" if reachable && !check.trace.is_empty() => {
                        witnesses.push(json!({
                            "status":"VIOLATED",
                            "property_id":format!("{id}@{}", expected.variant),
                            "steps":check.trace,
                            "native_reference":raw_ref
                        }));
                        ProofOutcome::Violated
                    }
                    _ => ProofOutcome::Unknown,
                }
            }
            Err(_) if actual.is_none() => ProofOutcome::NotChecked,
            Err(_) => ProofOutcome::Failed,
        };
        conclusive &= matches!(outcome, ProofOutcome::Proven | ProofOutcome::Violated);
        run.properties.push(Property {
            id: format!("{id}@{}", expected.variant),
            description: format!("{}: {}", obligation.requirement, obligation.proposition),
            scope: format!("Kani/CBMC harness {harness}, variant {}", expected.variant),
            completeness_established: model_complete,
            outcome,
            assumptions: obligation.assumptions.clone(),
            unwind_bound: Some(*unwind),
            native_references: references,
        });
    }
    parsed?;
    ensure!(conclusive, "one or more proof obligations remain open");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn imports_reachable_proofs_and_rejects_stale_scope() -> Result<()> {
        let workspace = tempfile::tempdir()?;
        let root = workspace.path();
        let c = Config::new("interlock".into());
        let command = ["kani", "reset_transition"];
        let toolchain = json!({"rustc":"test","cargo":"test","kani":"test","cbmc":"test"});
        let plan = json!({"version":1,"invocations":{"reset":{
            "variant":"host","command":command,"toolchain":toolchain,
            "checks":{"format":"kani_cbmc","harness":"reset_transition","unwind":3,
            "completeness":["unwind"],"safety_checks":[],
            "obligations":{"keep":{"requirement":"REQ-KEEP","proposition":"fault implies inhibit",
            "native_check":"keep","reachability":["reached"],"assumptions":[]}}}
        }}});
        std::fs::write(root.join(PLAN), serde_json::to_vec(&plan)?)?;
        std::fs::write(root.join("lib.rs"), "fn transition() {}")?;
        let inputs = snapshot(root, &c)?;
        let bundle = Path::new("bundle.json");

        let cases = [
            ("SUCCESS", "SUCCESS", true, ProofOutcome::Proven, 0),
            ("FAILURE", "SUCCESS", true, ProofOutcome::Violated, 1),
            ("SUCCESS", "FAILURE", true, ProofOutcome::Unknown, 3),
            ("SUCCESS", "SUCCESS", false, ProofOutcome::Unknown, 3),
        ];
        for (claim, unwind, reached, outcome, exit) in cases {
            let output = json!([{"result":[
                {"property":"reached","status":if reached {"FAILURE"} else {"SUCCESS"},"trace":[{}]},
                {"property":"unwind","status":unwind},
                {"property":"keep","status":claim,"trace":[{}]}
            ]},{"cProverStatus":if reached || unwind == "FAILURE" || claim == "FAILURE" {"failure"} else {"success"}}]).to_string();
            let record = json!({"version":1,"producer":"synthetic fixture","inputs":inputs,
                "invocations":{"reset":{"command":command,"toolchain":toolchain,
                "exit_code":if output.contains("\"failure\"") {10} else {0},
                "output_sha256":evidence::hash(output.as_bytes()),"output":output}}});
            std::fs::write(root.join(bundle), serde_json::to_vec(&record)?)?;
            let run = import(root, &c, bundle)?;
            assert_eq!(run.exit_code(), exit, "{:?}", run.limitations);
            let saved = evidence::load(root, &c, &run.id)?;
            assert_eq!(saved.properties[0].outcome, outcome);
        }
        std::fs::write(root.join("lib.rs"), "fn changed() {}")?;
        let stale = import(root, &c, bundle)?;
        assert_eq!(stale.exit_code(), 3);
        assert_eq!(stale.properties[0].outcome, ProofOutcome::NotChecked);
        Ok(())
    }

    #[test]
    fn clippy_requires_fresh_artifact_for_expected_scope() -> Result<()> {
        let workspace = tempfile::tempdir()?;
        let root = workspace.path();
        let c = Config::new("interlock".into());
        let command = ["cargo", "clippy"];
        let toolchain = json!({"rustc":"test","cargo":"test","clippy":"test"});
        let plan = json!({"version":1,"invocations":{
            "host":{"variant":"host","command":command,"toolchain":toolchain,
            "checks":{"format":"clippy","package":"local","target":"interlock","kind":"lib"}},
            "embedded":{"variant":"embedded","command":["cargo","clippy","--target","thumbv7em-none-eabihf"],"toolchain":toolchain,
            "checks":{"format":"clippy","package":"local","target":"interlock","kind":"lib"}}
        }});
        std::fs::write(root.join(PLAN), serde_json::to_vec(&plan)?)?;
        let inputs = snapshot(root, &c)?;
        let bundle = Path::new("bundle.json");
        let output = concat!(
            "{\"reason\":\"compiler-artifact\",\"package_id\":\"local\",\"target\":{\"name\":\"interlock\",\"kind\":[\"lib\"]},\"fresh\":false}\n",
            "{\"reason\":\"build-finished\",\"success\":true}\n"
        );
        let record = json!({"version":1,"producer":"synthetic fixture","inputs":inputs,
            "invocations":{"host":{"command":command,"toolchain":toolchain,"exit_code":0,
            "output_sha256":evidence::hash(output.as_bytes()),"output":output}}});
        std::fs::write(root.join(bundle), serde_json::to_vec(&record)?)?;
        let run = import(root, &c, bundle)?;
        assert_eq!(run.exit_code(), 3);
        assert_eq!(run.completeness.analyzed, 1);
        assert_eq!(run.completeness.failed, 1);
        Ok(())
    }
}
