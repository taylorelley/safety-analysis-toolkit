use crate::{
    config::{Config, inside},
    discovery, evidence,
    model::{Finding, Run, Status},
    runner,
};
use anyhow::{Context, Result};
use serde::Deserialize;
use serde_json::{Value, json};
use std::{collections::BTreeMap, fs, path::Path};
#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Provenance {
    version: u32,
    binary_sha256: String,
    profile_sha256: String,
    sources: BTreeMap<String, String>,
    compile_commands_sha256: String,
    execution_environment: String,
    build_variant: String,
    instrumentation_warnings: Vec<String>,
    exclusions: Vec<String>,
    test_ids: Vec<String>,
    requirement_ids: Vec<String>,
}
pub fn import(
    root: &Path,
    c: &Config,
    profile: &Path,
    binary: &Path,
    provenance: Option<&Path>,
) -> Result<Run> {
    let profile = inside(root, profile)?;
    let binary = inside(root, binary)?;
    let units = discovery::database(root, &c.build.compile_commands)?;
    let (mut run, dir) = evidence::begin(root, c, "coverage")?;
    run.completeness.total = units.len();
    run.completeness.build_variants = units.iter().map(|u| u.variant.clone()).collect();
    let Some(tool) = discovery::executable("llvm-cov-19", &c.tools.allowlist) else {
        run.completeness.failed = units.len();
        run.limitations
            .push("llvm-cov-19 unavailable or not allowlisted".into());
        return evidence::finish(root, c, &dir, run);
    };
    match runner::execute(
        &tool,
        &["--version".into()],
        root,
        &dir,
        c.tools.timeout_seconds,
        c.tools.memory_mb,
        c.tools.require_filesystem_isolation,
    ) {
        Ok(e) => {
            let compatible = e.failure.is_none() && e.stdout.contains("version 19.1.7");
            run.executions.push(e);
            if !compatible {
                run.completeness.unsupported = units.len();
                run.limitations
                    .push("Coverage import requires LLVM 19.1.7".into());
                return evidence::finish(root, c, &dir, run);
            }
        }
        Err(e) => {
            run.completeness.failed = units.len();
            run.limitations.push(e.to_string());
            return evidence::finish(root, c, &dir, run);
        }
    }
    let mut provenance_valid = false;
    if let Some(path) = provenance {
        let data = fs::read(inside(root, path)?)?;
        let p: Provenance = serde_json::from_slice(&data)?;
        provenance_valid = p.version == 1
            && ["host", "target"].contains(&p.execution_environment.as_str())
            && !p.build_variant.is_empty()
            && p.binary_sha256 == evidence::hash_file(&binary)?
            && p.profile_sha256 == evidence::hash_file(&profile)?
            && p.compile_commands_sha256
                == evidence::hash_file(&inside(root, &c.build.compile_commands)?)?
            && p.instrumentation_warnings.is_empty()
            && p.exclusions.is_empty();
        for (name, hash) in &p.sources {
            provenance_valid &= evidence::hash_file(&inside(root, Path::new(name))?)? == *hash;
        }
        for unit in &units {
            provenance_valid &= p
                .sources
                .contains_key(unit.file.strip_prefix(root)?.to_string_lossy().as_ref());
        }
        let _links = (&p.test_ids, &p.requirement_ids);
        evidence::write_new(&dir.join("coverage-provenance.json"), &data)?;
    }
    if !provenance_valid {
        run.limitations.push("Coverage provenance absent/inconsistent or contains instrumentation warnings/exclusions; build/execution scope is incomplete".into());
    }
    let args = vec![
        "export".into(),
        binary.display().to_string(),
        format!("--instr-profile={}", profile.display()),
    ];
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
            evidence::write_new(&dir.join("raw/llvm-cov.json"), e.stdout.as_bytes())?;
            evidence::write_new(&dir.join("raw/llvm-cov.log"), e.stderr.as_bytes())?;
            let parsed = (|| -> Result<Value> {
                let value: Value = serde_json::from_str(&e.stdout)?;
                anyhow::ensure!(
                    value["type"] == "llvm.coverage.json.export" && value["version"] == "2.0.1",
                    "unsupported LLVM coverage export schema"
                );
                let data = value["data"].as_array().context("missing coverage data")?;
                anyhow::ensure!(!data.is_empty(), "empty coverage data");
                for object in data {
                    for file in object["files"]
                        .as_array()
                        .context("missing coverage files")?
                    {
                        inside(
                            root,
                            Path::new(file["filename"].as_str().context("missing filename")?),
                        )?;
                        for metric in ["regions", "branches", "mcdc"] {
                            let stats = &file["summary"][metric];
                            let count = stats["count"].as_u64().context("missing metric count")?;
                            let covered =
                                stats["covered"].as_u64().context("missing covered count")?;
                            anyhow::ensure!(covered <= count, "inconsistent coverage counters");
                        }
                    }
                }
                Ok(value)
            })();
            if e.failure.is_none() {
                match parsed {
                    Ok(value) => {
                        let mut files = BTreeMap::new();
                        let mut mcdc_missing = false;
                        for object in value["data"].as_array().context("missing coverage data")? {
                            for file in object["files"]
                                .as_array()
                                .context("missing coverage files")?
                            {
                                let name = file["filename"].as_str().context("missing filename")?;
                                let path = inside(root, Path::new(name))?;
                                files.insert(path.clone(), file.clone());
                                for metric in ["regions", "branches", "mcdc"] {
                                    let stats = &file["summary"][metric];
                                    let count = stats["count"].as_u64().unwrap_or(0);
                                    let covered = stats["covered"].as_u64().unwrap_or(0);
                                    if covered < count {
                                        run.findings.push(Finding{id:String::new(),tool:"llvm-cov-19".into(),rule_id:format!("coverage-gap-{metric}"),file:path.display().to_string(),line:1,column:1,severity:"warning".into(),message:format!("{metric}: {covered}/{count} measured items covered; investigate linked requirements and requirements-based tests"),build_variant:"coverage-artifact".into(),outcome:"DIAGNOSTIC".into(),native_references:vec!["raw/llvm-cov.json".into()]});
                                    }
                                }
                                if file["summary"]["branches"]["count"].as_u64().unwrap_or(0) > 0
                                    && file["summary"]["mcdc"]["count"].as_u64().unwrap_or(0) == 0
                                {
                                    mcdc_missing = true;
                                }
                            }
                        }
                        for unit in &units {
                            if units.iter().filter(|u| u.file == unit.file).count() > 1 {
                                run.completeness.unsupported += 1;
                                run.limitations.push(format!("Profile does not establish which compilation variant of {} was instrumented",unit.file.display()));
                            } else if files.contains_key(&unit.file) {
                                run.completeness.analyzed += 1
                            } else {
                                run.completeness.excluded += 1
                            }
                        }
                        if mcdc_missing {
                            run.limitations.push("Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown".into());
                        }
                        run.complete = run.completeness.complete()
                            && provenance_valid
                            && !mcdc_missing
                            && e.stderr.trim().is_empty();
                        evidence::write_new(
                            &dir.join("coverage.json"),
                            &serde_json::to_vec_pretty(
                                &json!({"binary":binary,"binary_sha256":evidence::hash_file(&binary)?,"profile":profile,"profile_sha256":evidence::hash_file(&profile)?,"export":value,"provenance_valid":provenance_valid}),
                            )?,
                        )?;
                    }
                    Err(error) => {
                        run.completeness.failed = units.len();
                        run.limitations.push(error.to_string());
                    }
                }
            } else {
                run.completeness.failed = units.len();
            }
            run.executions.push(e);
        }
        Err(e) => {
            run.completeness.failed = units.len();
            run.limitations.push(e.to_string());
        }
    }
    run.findings = crate::findings::correlate(run.findings, root);
    run.status = if !run.complete {
        Status::Incomplete
    } else if run.findings.is_empty() {
        Status::Passed
    } else {
        Status::Findings
    };
    run.limitations.push("Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.".into());
    evidence::finish(root, c, &dir, run)
}
