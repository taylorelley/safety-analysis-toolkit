use crate::model::Run;
use serde_json::{Value, json};
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
    s.push_str("\n## Formal properties\n\n");
    for p in &r.properties {
        s.push_str(&format!(
            "- {}: {:?}; {}. Scope: {}\n",
            p.id, p.outcome, p.description, p.scope
        ));
    }
    s.push_str("\n## Outstanding work and limitations\n\n");
    for l in &r.limitations {
        s.push_str(&format!("- {}\n", l))
    }
    s
}
pub fn sarif(r: &Run) -> Value {
    let results:Vec<Value>=r.findings.iter().map(|f|json!({"ruleId":f.rule_id,"level":if f.severity=="error"{"error"}else{"warning"},"message":{"text":f.message},"locations":[{"physicalLocation":{"artifactLocation":{"uri":uri(&f.file)},"region":{"startLine":f.line.max(1),"startColumn":f.column.max(1)}}}],"properties":{"buildVariant":f.build_variant,"outcome":f.outcome,"nativeReferences":f.native_references}})).collect();
    json!({"$schema":"https://json.schemastore.org/sarif-2.1.0.json","version":"2.1.0","runs":[{"tool":{"driver":{"name":"safety","version":env!("CARGO_PKG_VERSION")}},"results":results,"invocations":[{"executionSuccessful":r.complete}],"properties":{"status":r.status,"complete":r.complete,"limitations":r.limitations}}]})
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
    fn escapes_uri() {
        assert_eq!(uri("src/a b.c"), "src/a%20b.c");
    }
}
