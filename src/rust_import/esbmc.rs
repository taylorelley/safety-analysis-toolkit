use super::*;

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
pub(super) struct Spec {
    pub(super) cbmc: String,
    model: String,
    model_sha256: String,
    entry: String,
    properties: BTreeSet<String>,
    #[serde(default)]
    allowed_warnings: BTreeSet<String>,
}

impl Spec {
    pub(super) fn validate(&self, expected: &Expected, plan: &Plan) -> Result<()> {
        relative(Path::new(&self.model))?;
        let paired = plan
            .invocations
            .get(&self.cbmc)
            .context("missing paired CBMC invocation")?;
        let Checks::KaniCbmc { unwind, .. } = &paired.checks else {
            anyhow::bail!("ESBMC replay requires a Kani/CBMC peer");
        };
        let model = self.model.as_str();
        let entry = self.entry.as_str();
        let bound = unwind.to_string();
        ensure!(
            expected
                .toolchain
                .get("esbmc")
                .is_some_and(|v| v == "8.7.0")
                && !self.properties.is_empty()
                && !entry.is_empty()
                && self.model_sha256.len() == 64
                && self.model_sha256.bytes().all(|b| b.is_ascii_hexdigit())
                && expected.variant == paired.variant
                && expected.toolchain.contains_key("kani")
                && expected.toolchain.get("kani") == paired.toolchain.get("kani")
                && expected.toolchain.get("rustc") == paired.toolchain.get("rustc")
                && expected.command
                    == [
                        "esbmc",
                        "--binary",
                        model,
                        "--function",
                        entry,
                        "--ignore-property-class",
                        "reachability_check",
                        "--ignore-property-class",
                        "cover",
                        "--unwind",
                        &bound,
                        "--multi-property",
                    ]
                && paired.command
                    == [
                        "cbmc",
                        model,
                        "--function",
                        entry,
                        "--unwind",
                        &bound,
                        "--unwinding-assertions",
                        "--trace",
                        "--json-ui",
                    ]
                && self.allowed_warnings.iter().all(|w| w.starts_with(
                    "WARNING: no definition for external symbol signgam declared at file "
                ) || w.starts_with(
                    "WARNING: no definition for external symbol errno declared at file "
                )),
            "invalid paired ESBMC replay"
        );
        Ok(())
    }

    pub(super) fn record(
        &self,
        run: &mut Run,
        scope: Attempt<'_>,
        paired: Option<&[Property]>,
        witnesses: &mut Vec<Value>,
    ) -> Result<()> {
        let paired = paired.context("paired CBMC proof did not complete")?;
        let actual = scope
            .actual
            .filter(|_| run.inputs.get(&self.model) == Some(&self.model_sha256));
        let native = actual
            .context("missing ESBMC result or mismatched model digest")
            .and_then(|actual| self.results(actual));
        let mut agreed = true;
        for peer in paired {
            let reference = peer
                .native_references
                .iter()
                .find_map(|r| r.split_once("#property=").map(|(_, id)| id))
                .context("paired CBMC property ID missing")?;
            let id = format!("{}#esbmc", peer.id);
            let outcome = match &native {
                Ok((checks, false, log)) => match checks.get(reference) {
                    Some(("PASSED", _)) if peer.completeness_established && log.exit_code == 0 => {
                        ProofOutcome::Proven
                    }
                    Some(("FAILED", description)) => match witness(&log.output, description) {
                        Some(steps) => {
                            witnesses.push(json!({"status":"VIOLATED", "property_id":id, "steps":steps, "native_reference":scope.raw_ref}));
                            ProofOutcome::Violated
                        }
                        None => ProofOutcome::Unknown,
                    },
                    _ => ProofOutcome::Unknown,
                },
                Ok(_) => ProofOutcome::Unknown,
                Err(_) if actual.is_none() => ProofOutcome::NotChecked,
                Err(_) => ProofOutcome::Failed,
            };
            if outcome != peer.outcome {
                agreed = false;
                run.limitations.push(format!(
                    "Backend results differ for {}: CBMC {}, ESBMC {}",
                    peer.id,
                    peer.outcome.as_str(),
                    outcome.as_str()
                ));
            }
            run.properties.push(Property {
                id,
                description: peer.description.clone(),
                scope: format!(
                    "ESBMC 8.7.0 GOTO replay at {}; paired CBMC reachability evidence",
                    self.entry
                ),
                assumptions: peer.assumptions.clone(),
                unwind_bound: peer.unwind_bound,
                completeness_established: outcome == ProofOutcome::Proven,
                outcome,
                native_references: vec![
                    scope.raw_ref.into(),
                    format!("{}#property={reference}", scope.raw_ref),
                ],
            });
        }
        native?;
        ensure!(
            agreed,
            "ESBMC replay did not agree on every paired obligation"
        );
        Ok(())
    }

    fn results<'a>(&self, actual: &'a Invocation) -> Result<Native<'a>> {
        let terminal = actual
            .output
            .lines()
            .rev()
            .find(|l| !l.trim().is_empty() && !l.starts_with("ESBMC version 8.7.0 "));
        ensure!(
            (actual.exit_code == 0 && terminal == Some("VERIFICATION SUCCESSFUL"))
                || (actual.exit_code == 1 && terminal == Some("VERIFICATION FAILED")),
            "ESBMC did not finish with a supported result"
        );
        ensure!(
            actual.output.contains("CBMC goto-binary detected:")
                && actual
                    .output
                    .lines()
                    .chain(actual.diagnostics.iter().flat_map(|s| s.lines()))
                    .all(|line| {
                        let line = line.trim();
                        !line.starts_with("ERROR")
                            && (!line.starts_with("WARNING")
                                || self.allowed_warnings.contains(line))
                    }),
            "ESBMC rejected the model or emitted an unreviewed warning"
        );
        let (_, summary) = actual
            .output
            .split_once("** Results:")
            .context("missing ESBMC result table")?;
        let mut checks = BTreeMap::new();
        let mut unwind_failed = false;
        for line in summary.lines() {
            let Some(status @ ("PASSED" | "FAILED" | "UNKNOWN")) = line.split_whitespace().next()
            else {
                continue;
            };
            let (_, tail) = line.split_once('[').context("missing ESBMC property ID")?;
            let (id, description) = tail.split_once(']').context("invalid ESBMC property ID")?;
            unwind_failed |= status != "PASSED"
                && description
                    .to_ascii_lowercase()
                    .contains("unwinding assertion");
            ensure!(
                checks.insert(id, (status, description.trim())).is_none(),
                "repeated ESBMC property"
            );
        }
        ensure!(!checks.is_empty(), "empty ESBMC property table");
        if !unwind_failed {
            ensure!(
                checks.keys().all(|id| self.properties.contains(*id)),
                "unplanned ESBMC property"
            );
        }
        ensure!(
            actual.exit_code != 0
                || (checks.len() == self.properties.len()
                    && checks.values().all(|(status, _)| *status == "PASSED")),
            "inconsistent ESBMC success result"
        );
        Ok((checks, unwind_failed, actual))
    }
}

type Native<'a> = (BTreeMap<&'a str, (&'a str, &'a str)>, bool, &'a Invocation);

fn witness<'a>(log: &'a str, description: &str) -> Option<Vec<&'a str>> {
    let name = description
        .strip_prefix("line ")
        .and_then(|s| {
            s.split_once(char::is_whitespace)
                .map(|(_, tail)| tail.trim())
        })
        .unwrap_or(description);
    log.split("[Counterexample]").skip(1).find_map(|section| {
        let (states, violation) = section.split_once("Violated property:")?;
        (violation.lines().nth(2)?.trim() == name).then(|| {
            states
                .lines()
                .filter(|l| !l.trim().is_empty())
                .take(20)
                .collect()
        })
    })
}
