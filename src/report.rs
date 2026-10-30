use crate::model::Run;
use serde_json::{Value, json};
/// Bounded Markdown overview: status, completeness, finding counts by rule and
/// limitations, without listing individual findings.
/// One line of free text, at most `LINE_LIMIT` characters.
fn clip(text: &str) -> String {
    const LINE_LIMIT: usize = 240;
    let line = text.replace('\n', " ");
    match line.char_indices().nth(LINE_LIMIT) {
        Some((end, _)) => format!("{}...", &line[..end]),
        None => line,
    }
}
pub fn summary_markdown(r: &Run) -> String {
    const MAX_RULES: usize = 40;
    const MAX_LIMITATIONS: usize = 40;
    const MAX_PROPERTIES: usize = 50;
    // JSON-encoded bytes for the properties section; keeps the whole summary
    // well inside the 64 KiB MCP response limit even when quoting and escaping
    // enlarge the text.
    const PROPERTY_BUDGET: usize = 16 * 1024;
    let mut counts = std::collections::BTreeMap::<&str, usize>::new();
    for f in &r.findings {
        *counts.entry(f.rule_id.as_str()).or_default() += 1;
    }
    let mut counts: Vec<_> = counts.into_iter().collect();
    counts.sort_by(|a, b| b.1.cmp(&a.1).then(a.0.cmp(b.0)));
    let mut s = format!(
        "# Safety analysis summary\n\nProject: {}\n\nRun: {}\n\nStatus: {:?}; complete: {}\n\nTranslation units: {} total, {} analyzed, {} failed, {} unsupported, {} excluded.\n\n## Findings by rule ({} findings, {} rules)\n\n| Rule | Count |\n| --- | ---: |\n",
        r.project,
        r.id,
        r.status,
        r.complete,
        r.completeness.total,
        r.completeness.analyzed,
        r.completeness.failed,
        r.completeness.unsupported,
        r.completeness.excluded,
        r.findings.len(),
        counts.len()
    );
    for (rule, n) in counts.iter().take(MAX_RULES) {
        s.push_str(&format!("| `{}` | {n} |\n", clip(rule)));
    }
    if counts.len() > MAX_RULES {
        s.push_str(&format!(
            "\n{} further rules omitted.\n",
            counts.len() - MAX_RULES
        ));
    }
    if !r.properties.is_empty() {
        s.push_str("\n## Properties\n\n");
        let mut encoded = 0;
        let mut shown = 0;
        for p in r.properties.iter().take(MAX_PROPERTIES) {
            let line = format!("- {:?}: {}\n", p.outcome, clip(&p.description));
            // Count the line as it will appear inside the JSON response.
            let size = serde_json::to_string(&line).map_or(usize::MAX, |j| j.len() - 2);
            if encoded + size > PROPERTY_BUDGET {
                break;
            }
            s.push_str(&line);
            encoded += size;
            shown += 1;
        }
        if shown < r.properties.len() {
            s.push_str(&format!(
                "- {} further properties omitted; page through them with safety.get_run property_offset and property_limit\n",
                r.properties.len() - shown
            ));
        }
    }
    s.push_str("\n## Limitations\n\n");
    for l in r.limitations.iter().take(MAX_LIMITATIONS) {
        s.push_str(&format!("- {}\n", clip(l)));
    }
    if r.limitations.len() > MAX_LIMITATIONS {
        s.push_str(&format!(
            "- {} further limitations in evidence\n",
            r.limitations.len() - MAX_LIMITATIONS
        ));
    }
    s.push_str("\nUse safety.get_findings with rule_prefix, exclude_rule_prefixes, category or file filters to retrieve individual findings.\n");
    s
}
pub fn markdown(r: &Run) -> String {
    let mut s = format!(
        "# Safety analysis evidence\n\nProject: {}\n\nRun: {}\n\nStatus: {:?}; complete: {}\n\nTranslation units: {} total, {} analyzed, {} failed, {} unsupported, {} excluded.\n\n## Findings\n\n",
        r.project,
        r.id,
        r.status,
        r.complete,
        r.completeness.total,
        r.completeness.analyzed,
        r.completeness.failed,
        r.completeness.unsupported,
        r.completeness.excluded
    );
    for f in &r.findings {
        s.push_str(&format!(
            "- {}:{}:{} [{}] {} (variant {})\n",
            f.file,
            f.line,
            f.column,
            f.rule_id,
            f.message.replace('\n', " "),
            f.build_variant
        ))
    }
    s.push_str("\n## Selected profiles and scope\n\n");
    if r.configuration.is_null() {
        s.push_str("Configuration snapshot is unavailable in this legacy record.\n")
    } else {
        s.push_str(&format!(
            "Operation: {}. Assurance: `{}`. Coding editions: `{}`.\n\n",
            r.operation, r.configuration["assurance"], r.configuration["coding"]
        ));
    }
    s.push_str(&format!(
        "Input files fingerprinted: {}. Git metadata: `{}`. Build variants: {}.\n\n",
        r.inputs.len(),
        r.git
            .as_ref()
            .map_or("unavailable".into(), |g| g.to_string()),
        r.completeness.build_variants.join(", ")
    ));
    s.push_str("## Tools and provenance\n\n");
    s.push_str(&format!("Toolkit identity: `{}`.\n\n", r.toolkit));
    let mut tools = std::collections::BTreeSet::new();
    for e in &r.executions {
        if tools.insert(e.executable.clone()) {
            let version = r
                .executions
                .iter()
                .find(|v| {
                    v.executable == e.executable
                        && v.arguments
                            .iter()
                            .any(|a| a == "--version" || a == "-version")
                })
                .map(|v| v.stdout.trim())
                .filter(|s| !s.is_empty())
                .unwrap_or("version not separately probed");
            s.push_str(&format!(
                "- `{}`: {}; SHA-256 `{}`. Restrictions: {}.\n",
                e.executable,
                version.replace('\n', " "),
                e.executable_sha256,
                e.restrictions
            ));
        }
    }
    if r.configuration["coding"]["c"].is_string() || r.configuration["coding"]["cpp"].is_string() {
        s.push_str("\n## MISRA draft status\n\nNo guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.\n");
    }
    s.push_str("\n## Formal properties\n\n");
    for p in &r.properties {
        s.push_str(&format!("- {}: {}; {}. Scope: {}. Unwind bound: {:?}; model completeness established: {}. Supplied assumptions: {:?}. Evidence: {}.\n",p.id,p.outcome.as_str(),p.description,p.scope,p.unwind_bound,p.completeness_established,p.assumptions,p.native_references.join(", ")));
    }
    if r.operation == "coverage" {
        s.push_str("\n## Structural coverage\n\nMeasured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.\n");
    }
    if r.operation == "trace" {
        s.push_str("\n## Traceability\n\nRelationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.\n");
    }
    s.push_str("\n## Evidence references\n\nThis run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.\n");
    s.push_str("\n## Outstanding work and limitations\n\n");
    for l in &r.limitations {
        s.push_str(&format!("- {}\n", l))
    }
    s
}
pub fn sarif(r: &Run) -> Value {
    let results:Vec<Value>=r.findings.iter().map(|f|json!({"ruleId":f.rule_id,"level":if f.severity=="error"{"error"}else{"warning"},"message":{"text":f.message},"locations":[{"physicalLocation":{"artifactLocation":{"uri":uri(&f.file)},"region":region(f)}}],"properties":{"buildVariant":f.build_variant,"outcome":f.outcome,"nativeReferences":f.native_references}})).collect();
    json!({"$schema":"https://json.schemastore.org/sarif-2.1.0.json","version":"2.1.0","runs":[{"tool":{"driver":{"name":"safety","version":env!("CARGO_PKG_VERSION")}},"results":results,"invocations":[{"executionSuccessful":r.complete}],"properties":{"status":r.status,"complete":r.complete,"limitations":r.limitations}}]})
}
fn region(f: &crate::model::Finding) -> Value {
    let mut v = json!({"startLine":f.line.max(1),"startColumn":f.column.max(1)});
    if let Some(line) = f.end_line.filter(|line| *line >= f.line.max(1)) {
        v["endLine"] = json!(line);
        if let Some(column) = f
            .end_column
            .filter(|column| *column >= 1 && (line > f.line || *column >= f.column))
        {
            v["endColumn"] = json!(column);
        }
    }
    v
}
fn uri(path: &str) -> String {
    let mut out = String::new();
    for b in path.bytes() {
        if b.is_ascii_alphanumeric() || b"/-._~:".contains(&b) {
            out.push(b as char)
        } else {
            out.push_str(&format!("%{b:02X}"))
        }
    }
    out
}
pub fn consolidated(
    root: &std::path::Path,
    c: &crate::config::Config,
) -> anyhow::Result<(serde_json::Value, String, i32)> {
    let base = crate::config::inside(root, &c.evidence.directory)?;
    let mut ids = std::fs::read_dir(base)?
        .map(|e| e.map(|e| e.file_name().to_string_lossy().into_owned()))
        .collect::<std::io::Result<Vec<_>>>()?;
    ids.sort();
    anyhow::ensure!(!ids.is_empty(), "no persisted runs");
    let mut runs = Vec::new();
    let mut errors = Vec::new();
    let mut text = String::from(
        "# Consolidated safety evidence\n\nEach operation retains its own scope and build variants. This is draft evidence, not a compliance or certification decision.\n\n",
    );
    let mut code = 0;
    for id in ids {
        match crate::evidence::load(root, c, &id) {
            Ok(r) => {
                code = code.max(r.exit_code());
                text.push_str(&markdown(&r));
                text.push('\n');
                runs.push(r)
            }
            Err(e) => {
                code = 3;
                errors.push(json!({"run_id":id,"error":e.to_string(),"status":"incomplete","complete":false}));
                text.push_str(&format!("Run {id}: incomplete evidence: {e}\n\n"));
            }
        }
    }
    Ok((
        json!({"schema_version":1,"status":if code==3{"incomplete"}else if code==1{"findings"}else{"passed"},"complete":code!=3,"runs":runs,"evidence_errors":errors}),
        text,
        code,
    ))
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn summary_stays_bounded_for_many_long_properties() {
        let long = "é".repeat(5000);
        let properties: Vec<_> = (0..1000)
            .map(|i| json!({"id":format!("P{i}"),"description":long,"outcome":"UNKNOWN","scope":"s","assumptions":[],"unwind_bound":null,"completeness_established":false,"native_references":[]}))
            .collect();
        let run: Run = serde_json::from_value(json!({"schema_version":1,"id":"RUN-1","operation":"prove","project":"p","status":"incomplete","complete":false,"completeness":{"total":1,"analyzed":1,"failed":0,"excluded":0,"unsupported":0,"build_variants":[]},"started_unix_ms":0,"completed_unix_ms":0,"inputs":{},"executions":[],"findings":[],"properties":properties,"limitations":vec![long.clone(); 100],"configuration":null,"verification":null,"toolkit":null,"git":null})).unwrap();
        let text = summary_markdown(&run);
        assert!(text.len() < 40 * 1024, "{}", text.len());
        assert!(text.contains("further properties omitted; page through them with safety.get_run"));
        // Escaping-heavy descriptions are budgeted at their JSON-encoded size.
        let quoted = "\\\"".repeat(2000);
        let mut escaped = run.clone();
        for p in &mut escaped.properties {
            p.description = quoted.clone();
        }
        let text = summary_markdown(&escaped);
        let section =
            &text[text.find("## Properties").unwrap()..text.find("## Limitations").unwrap()];
        let encoded = serde_json::to_string(section).unwrap();
        assert!(encoded.len() < 16 * 1024 + 256, "{}", encoded.len());
        assert!(text.contains("60 further limitations"));
    }
    #[test]
    fn escapes_uri() {
        assert_eq!(uri("src/a b.c"), "src/a%20b.c");
    }
}
