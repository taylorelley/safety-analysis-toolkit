use super::*;

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
pub(super) struct Spec {
    source: String,
    models: BTreeMap<String, String>,
    proof: String,
    probe: String,
    module: String,
    theorem: String,
    proposition: String,
    requirement: String,
    allowed_axioms: BTreeSet<String>,
}

impl Spec {
    pub(super) fn validate(&self, expected: &Expected) -> Result<()> {
        let inputs = [&self.source, &self.proof, &self.probe];
        ensure!(
            inputs.into_iter().collect::<BTreeSet<_>>().len() == inputs.len(),
            "Aeneas source, proof and probe must be distinct"
        );
        for path in inputs.into_iter().chain(self.models.keys()) {
            relative(Path::new(path))?;
        }
        let pins = [
            ("lean", "4.35.0"),
            ("hax", "0.5.0"),
            ("charon", "nightly-2026.10.31"),
            ("aeneas", "build-2b9e3ea"),
            ("hax-lean-lib", "v0.3.41"),
        ];
        ensure!(
            expected.command == ["lake", "env", "lean", "--json", &self.probe]
                && pins
                    .iter()
                    .all(|(tool, pin)| expected.toolchain.get(*tool).is_some_and(|v| v == pin))
                && self.module.split('.').all(lean_name)
                && self.theorem.split('.').all(lean_name)
                && !self.proposition.trim().is_empty()
                && !self.proposition.contains(['\r', '\n'])
                && !self.requirement.trim().is_empty()
                && !self.models.is_empty()
                && self
                    .models
                    .iter()
                    .all(|(path, digest)| !inputs.contains(&path)
                        && path.ends_with(".lean")
                        && digest.len() == 64
                        && digest.bytes().all(|b| b.is_ascii_hexdigit())),
            "invalid Aeneas/Lean invocation"
        );
        Ok(())
    }

    pub(super) fn record(&self, run: &mut Run, scope: Attempt<'_>) -> Result<()> {
        let probe = format!(
            "import {}\nset_option warningAsError true\n#check (@{} : {})\n#print axioms {}\n",
            self.module, self.theorem, self.proposition, self.theorem
        );
        let actual = scope.actual.filter(|_| {
            run.inputs.contains_key(&self.source)
                && run.inputs.contains_key(&self.proof)
                && run.inputs.get(&self.probe) == Some(&evidence::hash(probe.as_bytes()))
                && self
                    .models
                    .iter()
                    .all(|(path, digest)| run.inputs.get(path) == Some(digest))
        });
        let mut axioms = BTreeSet::new();
        let result = actual
            .context("missing Lean result or mismatched snapshot")
            .and_then(|actual| self.outcome(actual, &mut axioms));
        let outcome = mapped(&result, actual);
        let mut references = vec![scope.raw_ref.into()];
        if actual.is_some_and(|a| a.diagnostics.is_some()) {
            references.push(format!("{}.stderr", scope.raw_ref));
        }
        run.properties.push(Property {
            id: format!("{}@{}", scope.id, scope.variant),
            description: format!("{}: {}", self.requirement, self.proposition),
            scope: format!(
                "Lean 4.35.0 theorem {} over an Aeneas model of {}",
                self.theorem, self.source
            ),
            assumptions: axioms.into_iter().collect(),
            unwind_bound: None,
            completeness_established: outcome == ProofOutcome::Proven,
            outcome,
            native_references: references,
        });
        ensure!(
            result? == ProofOutcome::Proven,
            "Lean did not close the theorem with the reviewed axioms"
        );
        Ok(())
    }

    fn outcome(&self, actual: &Invocation, axioms: &mut BTreeSet<String>) -> Result<ProofOutcome> {
        ensure!(
            actual.regenerated_models.as_ref() == Some(&self.models),
            "regenerated Aeneas models differ from the reviewed extraction"
        );
        let records: Vec<Value> = actual
            .output
            .lines()
            .map(serde_json::from_str)
            .collect::<serde_json::Result<_>>()
            .context("malformed Lean JSON messages")?;
        if actual.exit_code == 1 && records.iter().any(|record| record["severity"] == "error") {
            return Ok(ProofOutcome::Unknown);
        }
        ensure!(
            actual.exit_code == 0
                && records.len() == 2
                && records.iter().all(|r| r["severity"] == "information"
                    && r["fileName"]
                        .as_str()
                        .is_some_and(|path| Path::new(path).ends_with(&self.probe)))
                && records[0]["pos"]["line"] == 3
                && records[1]["pos"]["line"] == 4,
            "missing or unexpected Lean result from the reviewed probe"
        );
        let report = records[1]["data"]
            .as_str()
            .context("missing Lean axiom report")?;
        if report != format!("'{}' does not depend on any axioms", self.theorem) {
            let list = report
                .strip_prefix(&format!("'{}' depends on axioms: [", self.theorem))
                .and_then(|s| s.strip_suffix(']'))
                .context("missing named theorem axiom inventory")?;
            axioms.extend(list.split(',').map(str::trim).map(str::to_owned));
        }
        Ok(
            if !axioms.contains("sorryAx") && axioms.is_subset(&self.allowed_axioms) {
                ProofOutcome::Proven
            } else {
                ProofOutcome::Unknown
            },
        )
    }
}

fn lean_name(part: &str) -> bool {
    let mut chars = part.chars();
    chars
        .next()
        .is_some_and(|c| c.is_ascii_alphabetic() || c == '_')
        && chars.all(|c| c.is_ascii_alphanumeric() || c == '_')
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn only_reviewed_theorem_and_axioms_close_the_obligation() -> Result<()> {
        let spec: Spec = serde_json::from_value(json!({
            "source":"lib.rs","models":{"Model.lean":"hash"},"proof":"Proof.lean",
            "probe":"Probe.lean","module":"Proof","theorem":"refines",
            "proposition":"True","requirement":"REQ-KEEP","allowed_axioms":["propext","sorryAx"]
        }))?;
        for (report, expected) in [
            (
                "'refines' does not depend on any axioms",
                ProofOutcome::Proven,
            ),
            (
                "'refines' depends on axioms: [propext]",
                ProofOutcome::Proven,
            ),
            (
                "'refines' depends on axioms: [sorryAx]",
                ProofOutcome::Unknown,
            ),
            (
                "'refines' depends on axioms: [unchecked]",
                ProofOutcome::Unknown,
            ),
        ] {
            let mut actual: Invocation = serde_json::from_value(json!({
                "toolchain":{},"command":[],"exit_code":0,"output_sha256":"",
                "output":([
                    json!({"fileName":"Probe.lean","pos":{"line":3},"severity":"information","data":"refines : True"}),
                    json!({"fileName":"Probe.lean","pos":{"line":4},"severity":"information","data":report})
                ].map(|line| line.to_string()).join("\n")),
                "regenerated_models":spec.models
            }))?;
            assert_eq!(spec.outcome(&actual, &mut BTreeSet::new())?, expected);
            actual.output = actual.output.replace("'refines'", "'other'");
            assert!(spec.outcome(&actual, &mut BTreeSet::new()).is_err());
            actual.regenerated_models = Some(BTreeMap::new());
            assert!(spec.outcome(&actual, &mut BTreeSet::new()).is_err());
        }
        Ok(())
    }
}
