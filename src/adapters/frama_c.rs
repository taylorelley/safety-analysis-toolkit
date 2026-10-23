use crate::{
    config::{Config, inside},
    discovery, evidence,
    model::{ProofOutcome, Property, Run},
    runner,
};
use anyhow::Result;
use std::path::Path;
pub fn prove(root: &Path, c: &Config, source: &Path) -> Result<Run> {
    let source = inside(root, source)?;
    anyhow::ensure!(
        source.extension().is_some_and(|e| e == "c"),
        "Frama-C adapter supports C only"
    );
    let (mut run, dir) = evidence::begin(root, c, "prove")?;
    run.completeness.total = 1;
    if let Some(tool) = discovery::executable("frama-c", &c.tools.allowlist) {
        match runner::execute(
            &tool,
            &["-version".into()],
            root,
            &dir,
            c.tools.timeout_seconds,
            c.tools.memory_mb,
            c.tools.require_filesystem_isolation,
        ) {
            Ok(e) => run.executions.push(e),
            Err(e) => run.limitations.push(e.to_string()),
        }
    }
    run.completeness.unsupported = 1;
    run.properties.push(Property {
        id: "eva-analysis".into(),
        description: "Frama-C Eva verification not checked".into(),
        outcome: ProofOutcome::NotChecked,
        scope: "No validated Frama-C installation/output adapter is available in this release"
            .into(),
        assumptions: Vec::new(),
        unwind_bound: None,
        completeness_established: false,
        native_references: Vec::new(),
    });
    run.limitations.push("Frama-C Eva/WP execution is unavailable pending a pinned, real integration test. No proof is inferred from installation or empty output.".into());
    evidence::finish(root, c, &dir, run)
}
