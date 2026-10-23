use crate::{
    config::{Config, inside},
    discovery, evidence,
    model::{Finding, Run, Status},
    runner,
};
use anyhow::{Context, Result, bail};
use serde::{Deserialize, Serialize};
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::Path,
};
#[derive(Debug, Deserialize, Serialize, Clone, PartialEq, Eq)]
#[serde(rename_all = "snake_case")]
pub enum Kind {
    Requirement,
    Design,
    Code,
    Test,
    Finding,
    Evidence,
    ChangeRequest,
    ProblemReport,
}
#[derive(Debug, Deserialize, Serialize, Clone)]
#[serde(deny_unknown_fields)]
pub struct Node {
    pub id: String,
    pub kind: Kind,
    #[serde(default)]
    pub root: bool,
    #[serde(default)]
    pub baseline: Option<String>,
}
#[derive(Debug, Deserialize, Serialize, Clone)]
#[serde(deny_unknown_fields)]
pub struct Link {
    pub from: String,
    pub to: String,
}
#[derive(Debug, Clone, Deserialize, Serialize)]
#[serde(deny_unknown_fields)]
pub struct Manifest {
    pub version: u32,
    #[serde(default)]
    pub baseline: Option<String>,
    pub nodes: Vec<Node>,
    pub links: Vec<Link>,
}
#[derive(Debug, Deserialize, Serialize, Clone)]
pub struct Symbol {
    pub usr: String,
    pub name: String,
    pub file: String,
    pub line: usize,
    pub column: usize,
    pub definition: bool,
    pub comment: String,
    pub macro_expansion: bool,
    pub template_parent: String,
    #[serde(default)]
    pub template_kind: String,
    #[serde(default)]
    pub calls: Vec<String>,
    #[serde(default)]
    pub unresolved_calls: usize,
    #[serde(default)]
    pub build_group: String,
    #[serde(default)]
    pub variant: String,
}
fn tokens(comment: &str, tag: &str) -> Vec<String> {
    let mut result = Vec::new();
    let mut words = comment.split_whitespace();
    while let Some(word) = words.next() {
        if word == tag
            && let Some(id) = words.next()
        {
            result.push(id.trim_end_matches("*/").to_string());
        }
    }
    result
}
fn allowed(a: &Kind, b: &Kind) -> bool {
    matches!(
        (a, b),
        (
            Kind::Requirement,
            Kind::Requirement | Kind::Design | Kind::Code | Kind::Test
        ) | (Kind::Design, Kind::Code)
            | (Kind::Test, Kind::Evidence)
            | (Kind::Code, Kind::Finding)
            | (Kind::Finding, Kind::ProblemReport)
            | (Kind::ChangeRequest, _)
    )
}
pub(crate) fn reachable(from: &str, to: &str, links: &[Link]) -> bool {
    let mut todo = vec![from];
    let mut seen = BTreeSet::new();
    while let Some(id) = todo.pop() {
        if !seen.insert(id) {
            continue;
        }
        if id == to {
            return true;
        }
        todo.extend(links.iter().filter(|l| l.from == id).map(|l| l.to.as_str()));
    }
    false
}
fn validate(manifest: &mut Manifest, symbols: &[Symbol]) -> Vec<(String, Option<Symbol>)> {
    let mut gaps = Vec::new();
    let mut nodes = BTreeMap::new();
    for n in &manifest.nodes {
        if nodes.insert(n.id.clone(), n.kind.clone()).is_some() {
            gaps.push((format!("Duplicate identifier {}", n.id), None))
        }
        if n.baseline.is_some() && n.baseline != manifest.baseline {
            gaps.push((format!("Baseline mismatch for {}", n.id), None))
        }
    }
    for s in symbols.iter().filter(|s| s.definition) {
        nodes.entry(s.usr.clone()).or_insert(Kind::Code);
    }
    let mut signatures: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for s in symbols.iter().filter(|s| s.definition) {
        signatures.entry(s.usr.clone()).or_default().insert(format!(
            "{:?}|{:?}|{:?}|{:?}",
            tokens(&s.comment, "@req"),
            tokens(&s.comment, "@design"),
            tokens(&s.comment, "@test"),
            tokens(&s.comment, "@baseline")
        ));
    }
    for s in symbols.iter().filter(|s| s.definition) {
        if s.unresolved_calls > 0 {
            gaps.push((
                format!(
                    "{} indirect, virtual or lambda call relationships unresolved for {}",
                    s.unresolved_calls, s.name
                ),
                Some(s.clone()),
            ));
        }
        if signatures.get(&s.usr).is_some_and(|set| set.len() > 1) {
            gaps.push(("Annotations differ across translation-unit contexts; automatic mapping is ambiguous".into(),Some(s.clone())));
            continue;
        }
        let tests = tokens(&s.comment, "@test");
        let targets = if tests.is_empty() {
            vec![s.usr.clone()]
        } else {
            tests.clone()
        };
        for test in &tests {
            if nodes.get(test) != Some(&Kind::Test) {
                gaps.push((format!("Unknown test annotation {test}"), Some(s.clone())))
            }
        }
        for (tag, kind) in [("@req", Kind::Requirement), ("@design", Kind::Design)] {
            for id in tokens(&s.comment, tag) {
                if nodes.get(&id) != Some(&kind) {
                    gaps.push((
                        format!("Unknown or mistyped {tag} identifier {id}"),
                        Some(s.clone()),
                    ));
                    continue;
                }
                for target in &targets {
                    if kind == Kind::Design && !tests.is_empty() {
                        gaps.push((
                            "Design annotation on test symbol is ambiguous".into(),
                            Some(s.clone()),
                        ))
                    } else if !s.macro_expansion {
                        manifest.links.push(Link {
                            from: id.clone(),
                            to: target.clone(),
                        })
                    } else {
                        gaps.push(("Annotation on macro-generated symbol requires explicit manifest mapping".into(),Some(s.clone())))
                    }
                }
            }
        }
        for id in tokens(&s.comment, "@baseline") {
            if manifest.baseline.as_deref() != Some(&id) {
                gaps.push((
                    format!("Stale or unknown baseline annotation {id}"),
                    Some(s.clone()),
                ))
            }
        }
    }
    let mut valid = Vec::new();
    for l in &manifest.links {
        match (nodes.get(&l.from), nodes.get(&l.to)) {
            (Some(a), Some(b)) if allowed(a, b) => valid.push(l.clone()),
            (Some(_), Some(_)) => {
                gaps.push((format!("Invalid relationship {} -> {}", l.from, l.to), None))
            }
            _ => gaps.push((format!("Broken reference {} -> {}", l.from, l.to), None)),
        }
    }
    for n in &manifest.nodes {
        if n.kind == Kind::Requirement
            && valid.iter().any(|l| {
                l.from == n.id
                    && nodes.get(&l.to) == Some(&Kind::Requirement)
                    && reachable(&l.to, &n.id, &valid)
            })
        {
            gaps.push((
                format!("Cyclic requirement parent relationship at {}", n.id),
                None,
            ));
        }
        if n.kind == Kind::Requirement {
            if !n.root
                && !valid
                    .iter()
                    .any(|l| l.to == n.id && nodes.get(&l.from) == Some(&Kind::Requirement))
            {
                gaps.push((format!("Requirement {} has no parent", n.id), None))
            }
            for (kind, label) in [(Kind::Code, "implementation"), (Kind::Test, "test")] {
                if !nodes
                    .iter()
                    .any(|(id, k)| *k == kind && reachable(&n.id, id, &valid))
                {
                    gaps.push((format!("Requirement {} has no linked {label}", n.id), None))
                }
            }
        } else if n.kind == Kind::Test
            && !nodes
                .iter()
                .any(|(id, k)| *k == Kind::Requirement && reachable(id, &n.id, &valid))
        {
            gaps.push((format!("Test {} has no known requirement", n.id), None))
        }
    }
    for s in symbols
        .iter()
        .filter(|s| s.definition && tokens(&s.comment, "@test").is_empty())
    {
        if !nodes
            .iter()
            .any(|(id, k)| *k == Kind::Requirement && reachable(id, &s.usr, &valid))
        {
            gaps.push((format!("Untraced code symbol {}", s.name), Some(s.clone())))
        }
    }
    gaps
}
pub(crate) fn load_manifest(root: &Path, path: &Path) -> Result<Manifest> {
    let path = inside(root, path)?;
    let text = String::from_utf8(crate::config::read_bounded(&path, 4 * 1024 * 1024)?)?;
    let text = if text.starts_with("---\n") {
        text.strip_prefix("---\n")
            .unwrap()
            .split_once("\n---")
            .context("missing Markdown YAML frontmatter terminator")?
            .0
    } else {
        &text
    };
    let m: Manifest = serde_yaml::from_str(text)?;
    if m.version != 1 {
        bail!("unsupported trace manifest version")
    };
    Ok(m)
}
pub fn check_coverage_links(
    root: &Path,
    c: &Config,
    requirements: &[String],
    tests: &[String],
) -> Result<()> {
    if requirements.is_empty() && tests.is_empty() {
        return Ok(());
    }
    let manifest = load_manifest(root, &c.trace.manifest)?;
    for req in requirements {
        anyhow::ensure!(
            manifest
                .nodes
                .iter()
                .any(|n| n.id == *req && n.kind == Kind::Requirement),
            "unknown coverage requirement {req}"
        );
        anyhow::ensure!(
            tests
                .iter()
                .any(|test| reachable(req, test, &manifest.links)),
            "coverage requirement {req} has no linked supplied test"
        );
    }
    for test in tests {
        anyhow::ensure!(
            manifest
                .nodes
                .iter()
                .any(|n| n.id == *test && n.kind == Kind::Test),
            "unknown coverage test {test}"
        );
        anyhow::ensure!(
            requirements
                .iter()
                .any(|req| reachable(req, test, &manifest.links)),
            "coverage test {test} has no linked supplied requirement"
        );
    }
    Ok(())
}
pub fn analyze(root: &Path, c: &Config) -> Result<Run> {
    let units = discovery::database(root, &c.build.compile_commands)?;
    let (mut run, dir) = evidence::begin(root, c, "trace")?;
    run.completeness.total = units.len();
    run.completeness.build_variants = units.iter().map(|u| u.variant.clone()).collect();
    let manifest = match load_manifest(root, &c.trace.manifest) {
        Ok(m) => m,
        Err(e) => {
            run.completeness.failed = units.len();
            run.limitations
                .push(format!("Trace manifest unavailable or invalid: {e}"));
            return evidence::finish(root, c, &dir, run);
        }
    };
    let Some(tool) = discovery::executable("safety-source-index", &c.tools.allowlist) else {
        run.completeness.failed = units.len();
        run.limitations
            .push("safety-source-index unavailable or not allowlisted".into());
        return evidence::finish(root, c, &dir, run);
    };
    let mut symbols = Vec::new();
    let mut groups = BTreeSet::new();

    for (i, u) in units.iter().enumerate() {
        if runner::cancelled() {
            run.completeness.failed += units.len() - i;
            break;
        }
        if !u.unsupported.is_empty() {
            run.completeness.unsupported += 1;
            run.limitations.extend(u.unsupported.clone());
            continue;
        }
        let matched: Vec<_> = c
            .build
            .variants
            .iter()
            .filter(|(_, defs)| defs.iter().all(|d| u.definitions.contains(d)))
            .map(|(name, _)| name.clone())
            .collect();
        let group = if matched.len() == 1 {
            matched[0].clone()
        } else if c.build.variants.is_empty()
            && units.iter().filter(|unit| unit.file == u.file).count() == 1
        {
            "default".into()
        } else {
            run.completeness.unsupported += 1;
            run.limitations.push(format!("{}: define an unambiguous build.variants grouping before merging trace relationships",u.variant));
            continue;
        };
        groups.insert(group.clone());
        let work = dir.join(format!("unit-{i}"));
        fs::create_dir(&work)?;
        let db = serde_json::json!([{"directory":u.directory,"file":u.file,"arguments":std::iter::once(u.compiler.clone()).chain(u.arguments.clone()).collect::<Vec<_>>()}]);
        evidence::write_new(
            &work.join("compile_commands.json"),
            &serde_json::to_vec(&db)?,
        )?;
        match runner::execute(
            &tool,
            &[
                format!("-p={}", work.display()),
                format!(
                    "--dependencies={}",
                    work.join("dependencies.json").display()
                ),
                u.file.display().to_string(),
            ],
            &u.directory,
            &dir,
            c.tools.timeout_seconds,
            c.tools.memory_mb,
            c.tools.require_filesystem_isolation,
        ) {
            Ok(e) => {
                evidence::write_new(
                    &dir.join(format!("raw/index-{i}.jsonl")),
                    e.stdout.as_bytes(),
                )?;
                evidence::write_new(&dir.join(format!("raw/index-{i}.log")), e.stderr.as_bytes())?;
                let parsed = (|| -> Result<Vec<Symbol>> {
                    let dependencies: Vec<String> =
                        serde_json::from_slice(&crate::config::read_bounded(
                            &work.join("dependencies.json"),
                            4 * 1024 * 1024,
                        )?)?;
                    let mut hashes = std::collections::BTreeMap::new();
                    for name in dependencies {
                        let path = u.directory.join(&name).canonicalize()?;
                        anyhow::ensure!(path.is_file(), "non-file dependency");
                        hashes.insert(path.display().to_string(), evidence::hash_file(&path)?);
                    }
                    evidence::write_new(
                        &work.join("dependency-hashes.json"),
                        &serde_json::to_vec_pretty(&hashes)?,
                    )?;
                    let mut out = Vec::new();
                    for line in e.stdout.lines() {
                        let mut s: Symbol = serde_json::from_str(line)?;
                        anyhow::ensure!(
                            !s.usr.is_empty(),
                            "Clang could not provide an unambiguous symbol identifier"
                        );
                        s.file = inside(root, &u.directory.join(&s.file))?
                            .display()
                            .to_string();
                        s.variant = u.variant.clone();
                        s.build_group = group.clone();
                        out.push(s)
                    }
                    Ok(out)
                })();
                if e.failure.is_none() {
                    match parsed {
                        Ok(s) => {
                            symbols.extend(s);
                            run.completeness.analyzed += 1
                        }
                        Err(err) => {
                            run.limitations.push(err.to_string());
                            run.completeness.failed += 1
                        }
                    }
                } else {
                    run.completeness.failed += 1
                }
                run.record(e);
            }
            Err(e) => {
                run.completeness.failed += 1;
                run.limitations.push(e.to_string())
            }
        }
    }
    let mut graphs = std::collections::BTreeMap::new();
    for group in groups {
        let mut graph = manifest.clone();
        let members: Vec<_> = symbols
            .iter()
            .filter(|s| s.build_group == group)
            .cloned()
            .collect();
        let gaps = validate(&mut graph, &members);
        for (message, symbol) in gaps {
            let (file, line, column, variant) = if let Some(s) = symbol {
                (s.file, s.line, s.column, s.variant)
            } else {
                (c.trace.manifest.display().to_string(), 1, 1, group.clone())
            };
            run.findings.push(Finding {
                id: String::new(),
                tool: "safety-trace".into(),
                rule_id: format!("trace-{}", &evidence::hash(message.as_bytes())[..16]),
                file,
                line,
                column,
                severity: "warning".into(),
                message,
                build_variant: variant,
                outcome: "DIAGNOSTIC".into(),
                native_references: vec!["trace.json".into()],
                ..Default::default()
            });
        }

        graphs.insert(group, graph);
    }
    evidence::write_new(
        &dir.join("trace.json"),
        &serde_json::to_vec_pretty(
            &serde_json::json!({"manifest":manifest,"variant_graphs":graphs,"symbols":symbols}),
        )?,
    )?;
    run.findings = crate::findings::correlate(run.findings, root);
    run.complete = run.completeness.complete();
    run.status = if !run.complete {
        Status::Incomplete
    } else if run.findings.is_empty() {
        Status::Passed
    } else {
        Status::Findings
    };
    run.limitations.push("Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.".into());
    evidence::finish(root, c, &dir, run)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn gaps() {
        let mut m:Manifest=serde_yaml::from_str("version: 1\nnodes:\n- {id: R1, kind: requirement}\n- {id: T1, kind: test}\nlinks:\n- {from: MISSING, to: T1}\n").unwrap();
        let gaps = validate(&mut m, &[]);
        assert_eq!(gaps.len(), 5);
    }
    #[test]
    fn tokens_only() {
        assert_eq!(
            tokens("/** @req R1\n * @req R2 */", "@req"),
            vec!["R1", "R2"]
        );
    }
}
