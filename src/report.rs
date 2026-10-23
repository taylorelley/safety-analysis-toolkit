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
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn escapes_uri() {
        assert_eq!(uri("src/a b.c"), "src/a%20b.c");
    }
}
