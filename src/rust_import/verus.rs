use super::*;
use serde::Deserialize;

const VERSION: &str = "0.2026.11.02.8bf68ee";

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
pub(super) struct Spec {
    source: String,
    function: String,
    verified_functions: u64,
    requirement: String,
    proposition: String,
    #[serde(default)]
    assumptions: Vec<String>,
}

impl Spec {
    pub(super) fn validate(&self, expected: &Expected) -> Result<()> {
        let source = Path::new(&self.source);
        relative(source)?;
        ensure!(
            expected
                .toolchain
                .get("verus")
                .is_some_and(|v| v == VERSION)
                && expected.command
                    == [
                        "verus",
                        "--output-json",
                        "--no-cheating",
                        "--crate-type=lib",
                        &self.source
                    ]
                && self.function.split_once("::").is_some_and(|(krate, item)| {
                    !item.is_empty() && source.file_stem().and_then(|s| s.to_str()) == Some(krate)
                })
                && self.verified_functions > 0
                && !self.requirement.trim().is_empty()
                && !self.proposition.trim().is_empty(),
            "invalid full-crate Verus invocation"
        );
        Ok(())
    }

    pub(super) fn record(&self, run: &mut Run, scope: Attempt<'_>) -> Result<()> {
        let result = scope
            .actual
            .context("missing Verus invocation or inconsistent provenance")
            .and_then(|actual| {
                ensure!(
                    run.inputs.contains_key(&self.source),
                    "Verus source is absent from the input inventory"
                );
                self.outcome(actual)
            });
        let outcome = mapped(&result, scope.actual);
        let mut references = vec![scope.raw_ref.into()];
        if scope.actual.is_some_and(|a| a.diagnostics.is_some()) {
            references.push(format!("{}.stderr", scope.raw_ref));
        }
        run.properties.push(Property {
            id: format!("{}@{}", scope.id, scope.variant),
            description: format!("{}: {}", self.requirement, self.proposition),
            scope: format!(
                "Verus {VERSION}, full crate {}, function {}, variant {}",
                self.source, self.function, scope.variant
            ),
            assumptions: self.assumptions.clone(),
            unwind_bound: None,
            completeness_established: outcome == ProofOutcome::Proven,
            outcome,
            native_references: references,
        });
        ensure!(
            result? == ProofOutcome::Proven,
            "Verus left a verification condition open"
        );
        Ok(())
    }

    fn outcome(&self, actual: &Invocation) -> Result<ProofOutcome> {
        let native: Value = serde_json::from_str(&actual.output).context("malformed Verus JSON")?;
        let summary = &native["verification-results"];
        ensure!(
            native["verus"]["version"] == VERSION
                && summary["is-verifying-entire-crate"] == true
                && summary["encountered-vir-error"] == false
                && native["func-details"]
                    .get(&self.function)
                    .is_some_and(Value::is_object),
            "wrong Verus version, invalid crate or missing reviewed function"
        );
        match actual.exit_code {
            0 if summary["success"] == true
                && summary["encountered-error"] == false
                && summary["errors"] == 0
                && summary["verified"] == self.verified_functions =>
            {
                Ok(ProofOutcome::Proven)
            }
            1 if summary["success"] == false
                && summary["encountered-error"] == true
                && summary["errors"].as_u64().is_some_and(|n| n > 0) =>
            {
                Ok(ProofOutcome::Unknown)
            }
            _ => anyhow::bail!("inconsistent Verus result or verified function count"),
        }
    }
}
