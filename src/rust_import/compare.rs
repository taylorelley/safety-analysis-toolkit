use super::*;

pub fn runs(
    root: &Path,
    config: &Config,
    base: &str,
    head: &str,
    offset: usize,
    limit: usize,
) -> Result<(Value, i32)> {
    ensure!((1..=50).contains(&limit), "proof page limit must be 1..50");
    let baseline = evidence::load(root, config, base)?;
    let candidate = evidence::load(root, config, head)?;
    ensure!(
        baseline.operation == "rust-import"
            && candidate.operation == "rust-import"
            && baseline.project == candidate.project,
        "comparison requires Rust imports for the same project"
    );
    let base_plan = comparable_plan(root, config, base)?;
    let head_plan = comparable_plan(root, config, head)?;
    let baseline_props = by_id(&baseline)?;
    let candidate_props = by_id(&candidate)?;
    let ids: BTreeSet<&str> = baseline_props
        .keys()
        .chain(candidate_props.keys())
        .copied()
        .collect();
    ensure!(
        offset <= ids.len(),
        "proof page offset exceeds change count"
    );

    let mut counts = BTreeMap::<&str, usize>::new();
    let mut changes = Vec::new();
    for (index, id) in ids.iter().enumerate() {
        let previous = baseline_props.get(id).copied();
        let current = candidate_props.get(id).copied();
        let change = match (previous, current) {
            (Some(previous), Some(current)) if base_plan == head_plan => {
                classify(previous, current)
            }
            (None, Some(_)) => "added",
            (Some(_), None) => "removed",
            _ => "not_assessed",
        };
        *counts.entry(change).or_default() += 1;
        if index >= offset && changes.len() < limit {
            changes.push(json!({
                "id":id,"change":change,
                "before":previous.map(|p| &p.outcome),
                "after":current.map(|p| &p.outcome)
            }));
        }
    }
    let end = offset + changes.len();
    let exit = if counts.contains_key("regressed") {
        1
    } else if !baseline.complete
        || !candidate.complete
        || counts.contains_key("not_assessed")
        || counts.contains_key("removed")
        || base_plan != head_plan
    {
        3
    } else {
        0
    };
    Ok((
        json!({
            "base":baseline.id,"head":candidate.id,
            "base_complete":baseline.complete,"head_complete":candidate.complete,
            "reviewed_plan_matches":base_plan == head_plan,"counts":counts,
            "total":ids.len(),"offset":offset,"next_offset":(end < ids.len()).then_some(end),
            "changes":changes,"exit_code":exit
        }),
        exit,
    ))
}

fn comparable_plan(root: &Path, config: &Config, id: &str) -> Result<Value> {
    let path = config.evidence.directory.join(id).join(PLAN);
    let mut plan: Value =
        serde_json::from_slice(&read_bounded(&inside(root, &path)?, 1024 * 1024)?)?;
    let entries = plan["invocations"]
        .as_object_mut()
        .context("invalid archived Rust plan")?;
    for entry in entries.values_mut() {
        let checks = &mut entry["checks"];
        match checks["format"].as_str() {
            Some("esbmc_goto") => checks["model_sha256"] = Value::Null,
            Some("aeneas_lean") => {
                let models = checks["models"]
                    .as_object_mut()
                    .context("invalid archived model inventory")?;
                models.values_mut().for_each(|value| *value = Value::Null);
            }
            _ => {}
        }
    }
    Ok(plan)
}

fn by_id(run: &Run) -> Result<BTreeMap<&str, &Property>> {
    let mut props = BTreeMap::new();
    for prop in &run.properties {
        ensure!(
            props.insert(prop.id.as_str(), prop).is_none(),
            "duplicate proof ID in run {}",
            run.id
        );
    }
    Ok(props)
}

fn classify(before: &Property, after: &Property) -> &'static str {
    if before.description != after.description
        || before.scope != after.scope
        || before.unwind_bound != after.unwind_bound
    {
        "not_assessed"
    } else if before.outcome == ProofOutcome::Proven && after.outcome != ProofOutcome::Proven {
        "regressed"
    } else if before.assumptions != after.assumptions {
        "not_assessed"
    } else if before.outcome == after.outcome {
        "unchanged"
    } else if after.outcome == ProofOutcome::Proven {
        "improved"
    } else {
        "changed"
    }
}
