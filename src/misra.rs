use crate::{
    config::{Config, inside},
    model::Run,
};
use anyhow::{Context, Result};
use serde::{Deserialize, Serialize};
use serde_json::{Value, json};
use std::{
    collections::{BTreeMap, BTreeSet},
    path::Path,
};
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct Profile {
    edition: String,
    inventory_complete: bool,
    inventory_source: String,
    rules: Vec<Rule>,
    original_checks: Vec<PolicyCheck>,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct PolicyCheck {
    id: String,
    behavior: String,
    mapping: String,
    limitations: Vec<String>,
}
#[derive(Serialize, Deserialize, PartialEq)]
#[serde(rename_all = "snake_case")]
enum Coverage {
    Supported,
    Partial,
    Manual,
    NotImplemented,
    NotApplicable,
}
#[derive(Serialize, Deserialize, Clone, Copy, PartialEq)]
#[serde(rename_all = "snake_case")]
enum Category {
    Advisory,
    Required,
    Mandatory,
}
#[derive(Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
enum Decidability {
    Decidable,
    Undecidable,
}
#[derive(Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
enum AnalysisScope {
    SingleTranslationUnit,
    System,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct Rule {
    rule_id: String,
    #[serde(default)]
    category: Option<Category>,
    #[serde(default)]
    decidability: Option<Decidability>,
    #[serde(default)]
    analysis_scope: Option<AnalysisScope>,
    #[serde(default)]
    applies_to: Vec<String>,
    coverage: Coverage,
    implementation: Option<String>,
    automated_checks: Vec<String>,
    manual_review_required: bool,
    limitations: Vec<String>,
    validation: Option<MappingReview>,
    #[serde(default)]
    project_conditions: Vec<String>,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct MappingReview {
    source_ref: String,
    reviewed_by: String,
    timestamp: String,
    test_evidence: Vec<String>,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct Deviation {
    id: String,
    edition: String,
    rule_id: String,
    rationale: String,
    artifacts: Vec<String>,
    change_request: String,
    #[serde(default)]
    review: Option<Review>,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Review {
    pub reviewer: String,
    pub decision: String,
    pub rationale: String,
    pub timestamp: String,
    pub evidence: Vec<String>,
    pub change_record: String,
}
#[derive(Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct Recategorization {
    edition: String,
    rule_id: String,
    original_category: String,
    proposed_category: String,
    rationale: String,
}
fn load<T: serde::de::DeserializeOwned>(root: &Path, path: &Path) -> Result<T> {
    Ok(serde_yaml::from_slice(&crate::config::read_bounded(
        &inside(root, path)?,
        4 * 1024 * 1024,
    )?)?)
}
pub fn review_valid(root: &Path, r: &Review) -> Result<()> {
    anyhow::ensure!(
        !r.reviewer.trim().is_empty()
            && !r.decision.trim().is_empty()
            && !r.rationale.trim().is_empty()
            && !r.timestamp.trim().is_empty()
            && !r.change_record.trim().is_empty()
            && !r.evidence.is_empty(),
        "incomplete external review record"
    );
    for file in &r.evidence {
        inside(root, Path::new(file))?;
    }
    Ok(())
}
pub fn drafts(root: &Path, c: &Config, run: &Run) -> Result<Value> {
    let mut profiles = Vec::new();
    for edition in [c.coding.c.as_deref(), c.coding.cpp.as_deref()]
        .into_iter()
        .flatten()
    {
        let data = if edition == "MISRA-C:2012" {
            include_str!("../profiles/misra-c-2012.yaml")
        } else {
            include_str!("../profiles/misra-cpp-2023.yaml")
        };
        profiles.push(serde_yaml::from_str::<Profile>(data)?);
    }
    for path in &c.coding.registries {
        let profile: Profile = load(root, path)?;
        anyhow::ensure!(
            profiles.iter().any(|p| p.edition == profile.edition),
            "registry edition is not selected"
        );
        let pos = profiles
            .iter()
            .position(|p| p.edition == profile.edition)
            .unwrap();
        profiles[pos] = profile;
    }
    let mut entries = Vec::new();
    let mut known = BTreeSet::new();
    let mut categories = BTreeMap::new();
    for profile in &profiles {
        anyhow::ensure!(
            !profile.inventory_complete || !profile.rules.is_empty(),
            "complete inventory has no guidelines"
        );
        for rule in &profile.rules {
            anyhow::ensure!(
                !rule.rule_id.is_empty()
                    && known.insert((profile.edition.clone(), rule.rule_id.clone())),
                "empty or duplicate rule identifier"
            );
            if let Some(category) = rule.category {
                categories.insert((profile.edition.clone(), rule.rule_id.clone()), category);
            }
            if matches!(rule.coverage, Coverage::Supported | Coverage::Partial) {
                let review = rule.validation.as_ref().context(
                    "automated mapping requires externally reviewed source and test evidence",
                )?;
                anyhow::ensure!(
                    !review.source_ref.is_empty()
                        && !review.reviewed_by.is_empty()
                        && !review.timestamp.is_empty()
                        && !review.test_evidence.is_empty()
                        && !rule.automated_checks.is_empty()
                        && !rule.limitations.is_empty(),
                    "incomplete mapping scope or validation"
                );
                for path in &review.test_evidence {
                    inside(root, Path::new(path))?;
                }
            }
            if rule.coverage == Coverage::NotApplicable {
                anyhow::ensure!(
                    !rule.project_conditions.is_empty(),
                    "not-applicable guideline needs documented project conditions"
                );
            }
            let related: Vec<_> = run
                .findings
                .iter()
                .filter(|f| rule.automated_checks.contains(&f.rule_id))
                .map(|f| f.id.clone())
                .collect();
            let enabled = |id: &String| {
                run.executions.iter().any(|e| {
                    e.arguments
                        .iter()
                        .filter_map(|a| a.strip_prefix("--checks="))
                        .any(|setting| {
                            let mut selected = false;
                            for part in setting.split(',') {
                                let (negative, pattern) =
                                    part.strip_prefix('-').map_or((false, part), |p| (true, p));
                                let matches = pattern == "*"
                                    || pattern == id
                                    || pattern
                                        .strip_suffix('*')
                                        .is_some_and(|prefix| id.starts_with(prefix));
                                if matches {
                                    selected = !negative
                                }
                            }
                            selected
                        })
                })
            };
            let automated_performed = run.operation == "scan"
                && run.completeness.complete()
                && !rule.automated_checks.is_empty()
                && rule.automated_checks.iter().all(enabled);
            entries.push(json!({"edition":profile.edition,"rule_id":rule.rule_id,"category":rule.category,"decidability":rule.decidability,"analysis_scope":rule.analysis_scope,"applies_to":rule.applies_to,"coverage":rule.coverage,"automated_checks":rule.automated_checks,"manual_review_required":rule.manual_review_required,"limitations":rule.limitations,"project_conditions":rule.project_conditions,"mapping_review":rule.validation,"automated_execution":if automated_performed{"performed; inspect selected tool checks"}else{"not_established"},"related_findings":related,"compliance":"not_established; human guideline assessment required"}));
        }
    }
    let inventory: Vec<_> = profiles
        .iter()
        .map(|p| {
            let mut by_coverage = BTreeMap::new();
            for rule in &p.rules {
                *by_coverage
                    .entry(serde_json::to_value(&rule.coverage).unwrap().to_string())
                    .or_insert(0) += 1;
            }
            json!({"edition":p.edition,"inventory_complete":p.inventory_complete,"inventory_source":p.inventory_source,"guidelines":p.rules.len(),"by_coverage":by_coverage})
        })
        .collect();
    let deviations = if let Some(path) = &c.coding.deviations {
        load::<Vec<Deviation>>(root, path)?
    } else {
        Vec::new()
    };
    let mut ids = BTreeSet::new();
    for d in &deviations {
        anyhow::ensure!(
            ids.insert(&d.id)
                && !d.id.is_empty()
                && !d.rationale.is_empty()
                && !d.change_request.is_empty(),
            "invalid or duplicate deviation record"
        );
        anyhow::ensure!(
            known.contains(&(d.edition.clone(), d.rule_id.clone())),
            "deviation refers to unknown guideline"
        );
        for path in &d.artifacts {
            inside(root, Path::new(path))?;
        }
        if let Some(review) = &d.review {
            review_valid(root, review)?;
        }
    }
    let grp = if let Some(path) = &c.coding.recategorization {
        load::<Vec<Recategorization>>(root, path)?
    } else {
        Vec::new()
    };
    for r in &grp {
        let recorded = categories.get(&(r.edition.clone(), r.rule_id.clone()));
        let rank = |s: &str| match s {
            "advisory" => Some(Category::Advisory as u8),
            "required" => Some(Category::Required as u8),
            "mandatory" => Some(Category::Mandatory as u8),
            _ => None,
        };
        anyhow::ensure!(
            known.contains(&(r.edition.clone(), r.rule_id.clone()))
                && !r.rationale.is_empty()
                && rank(&r.original_category).is_some()
                && rank(&r.proposed_category) >= rank(&r.original_category)
                && recorded.is_none_or(|c| rank(&r.original_category) == Some(*c as u8)),
            "invalid, mismatched or weakening recategorization"
        );
    }
    Ok(
        json!({"status":"incomplete","complete":false,"artifact_status":"draft_for_human_review","profiles":profiles,"gep":{"guidelines":entries,"original_check_mapping":"unvalidated unless externally reviewed mapping is supplied"},"grp":{"proposals":grp,"approval":"not_granted_by_tool"},"gcs":{"inventory":inventory,"guidelines":entries,"compliance":"not_established","outstanding":["Verify licensed inventory and applicability","Complete manual checks and independent review","Resolve tool failures and incomplete scope"]},"deviations":{"records":deviations,"approved_by_tool":false,"external_reviews":"supplied process assertions, not authenticated by toolkit"}}),
    )
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn builtins_do_not_invent_mappings() {
        for text in [
            include_str!("../profiles/misra-c-2012.yaml"),
            include_str!("../profiles/misra-cpp-2023.yaml"),
        ] {
            let p: Profile = serde_yaml::from_str(text).unwrap();
            assert!(p.original_checks.iter().all(|c| c.mapping == "unvalidated"));
            assert!(p.rules.iter().all(|r| r.validation.is_none()
                && r.automated_checks.is_empty()
                && r.manual_review_required
                && !matches!(r.coverage, Coverage::Supported | Coverage::Partial)));
        }
        let cpp: Profile =
            serde_yaml::from_str(include_str!("../profiles/misra-cpp-2023.yaml")).unwrap();
        assert!(!cpp.inventory_complete && cpp.rules.is_empty());
    }
    #[test]
    fn misra_c_2012_inventory_lists_every_guideline() {
        let p: Profile =
            serde_yaml::from_str(include_str!("../profiles/misra-c-2012.yaml")).unwrap();
        assert!(p.inventory_complete);
        // Guidelines per section of the first edition: Dir 1-4, then Rule 1-22.
        let directives = [1, 1, 1, 13];
        let rules = [
            3, 7, 2, 2, 9, 2, 4, 14, 5, 8, 9, 4, 6, 4, 7, 7, 8, 8, 2, 14, 12, 6,
        ];
        let mut expected = Vec::new();
        for (kind, sizes) in [("Dir", &directives[..]), ("Rule", &rules[..])] {
            for (section, &n) in sizes.iter().enumerate() {
                expected.extend((1..=n).map(|i| format!("{kind} {}.{i}", section + 1)));
            }
        }
        let ids: Vec<_> = p.rules.iter().map(|r| r.rule_id.clone()).collect();
        assert_eq!(ids, expected);
        assert_eq!(ids.len(), 159);
        let count = |kind: &str, c: Category| {
            p.rules
                .iter()
                .filter(|r| r.rule_id.starts_with(kind) && r.category == Some(c))
                .count()
        };
        assert_eq!(count("Dir", Category::Mandatory), 0);
        assert_eq!(count("Dir", Category::Required), 9);
        assert_eq!(count("Dir", Category::Advisory), 7);
        assert_eq!(count("Rule", Category::Mandatory), 10);
        assert_eq!(count("Rule", Category::Required), 101);
        assert_eq!(count("Rule", Category::Advisory), 32);
        for r in &p.rules {
            let directive = r.rule_id.starts_with("Dir");
            assert_eq!(r.decidability.is_none(), directive, "{}", r.rule_id);
            assert_eq!(r.analysis_scope.is_none(), directive, "{}", r.rule_id);
            assert!(!r.applies_to.is_empty() && !r.limitations.is_empty());
            assert!(r.applies_to.iter().all(|v| v == "C90" || v == "C99"));
        }
    }
}
