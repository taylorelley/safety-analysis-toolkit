use crate::model::Finding;
use anyhow::{Context, Result};
use serde::Deserialize;
use std::{collections::BTreeMap, path::Path};
#[derive(Deserialize)]
struct Export {
    #[serde(rename = "Diagnostics", default)]
    diagnostics: Vec<Diagnostic>,
}
#[derive(Deserialize)]
struct Diagnostic {
    #[serde(rename = "DiagnosticName")]
    name: String,
    #[serde(rename = "DiagnosticMessage")]
    message: Message,
    #[serde(rename = "Level")]
    level: Option<String>,
}
#[derive(Deserialize)]
struct Message {
    #[serde(rename = "Message")]
    text: String,
    #[serde(rename = "FilePath")]
    file: String,
    #[serde(rename = "FileOffset")]
    offset: usize,
    #[serde(rename = "Ranges", default)]
    ranges: Vec<SourceRange>,
}
#[derive(Deserialize)]
struct SourceRange {
    #[serde(rename = "FilePath")]
    file: String,
    #[serde(rename = "FileOffset")]
    offset: usize,
    #[serde(rename = "Length")]
    length: usize,
}
pub fn parse(
    data: &[u8],
    variant: &str,
    reference: &str,
    directory: &Path,
) -> Result<Vec<Finding>> {
    let export: Export =
        serde_yaml::from_slice(data).context("invalid clang diagnostics export")?;
    // Sources and their newline offsets, read once per file rather than per diagnostic.
    let mut sources: BTreeMap<String, (usize, Vec<usize>)> = BTreeMap::new();
    let mut findings = Vec::new();
    for d in export.diagnostics {
        // Fail closed: a diagnostic that cannot be placed in source cannot be
        // reported, so the unit stays incomplete with a specific reason.
        anyhow::ensure!(
            !d.message.file.is_empty(),
            "diagnostic {} has no source location: {}",
            d.name,
            d.message.text
        );
        let file = directory.join(&d.message.file);
        if !sources.contains_key(&d.message.file) {
            let source = std::fs::read(&file)
                .with_context(|| format!("diagnostic source unavailable: {}", file.display()))?;
            let newlines = source
                .iter()
                .enumerate()
                .filter_map(|(i, b)| (*b == b'\n').then_some(i))
                .collect();
            sources.insert(d.message.file.clone(), (source.len(), newlines));
        }
        let (len, newlines) = &sources[&d.message.file];
        let len = *len;
        anyhow::ensure!(d.message.offset <= len, "diagnostic offset outside source");
        // One-based line and column of a byte offset.
        let position = |offset: usize| {
            let k = newlines.partition_point(|&n| n < offset);
            (
                k + 1,
                k.checked_sub(1)
                    .map_or(offset + 1, |k| offset - newlines[k]),
            )
        };
        let offset = d.message.offset;
        let end = d
            .message
            .ranges
            .iter()
            .find(|r| {
                r.file == d.message.file
                    && r.offset <= offset
                    && r.offset.saturating_add(r.length) >= offset
            })
            .map(|r| position(r.offset.saturating_add(r.length).min(len)));
        let (end_line, end_column) = (end.map(|p| p.0), end.map(|p| p.1));
        let (line, column) = position(offset);
        findings.push(Finding {
            id: String::new(),
            tool: "clang-tidy-19".into(),
            rule_id: d.name,
            file: file.display().to_string(),
            line,
            column,
            end_line,
            end_column,
            severity: d.level.unwrap_or("warning".into()).to_lowercase(),
            message: d.message.text,
            build_variant: variant.into(),
            outcome: "DIAGNOSTIC".into(),
            native_references: vec![reference.into()],
            ..Default::default()
        });
    }
    Ok(findings)
}
pub fn correlate(findings: Vec<Finding>, root: &Path) -> Vec<Finding> {
    // Only identical diagnostics merge: the message is part of the key, so two
    // different claims at one location are both kept.
    let mut unique: BTreeMap<(String, usize, usize, String, String, String), Finding> =
        BTreeMap::new();
    for mut f in findings {
        if let Ok(p) = Path::new(&f.file).strip_prefix(root) {
            f.file = p.to_string_lossy().into_owned()
        };
        let key = (
            f.file.clone(),
            f.line,
            f.column,
            f.rule_id.clone(),
            f.build_variant.clone(),
            f.message.clone(),
        );
        if let Some(existing) = unique.get_mut(&key) {
            existing.native_references.extend(f.native_references);
        } else {
            unique.insert(key, f);
        }
    }
    unique
        .into_values()
        .enumerate()
        .map(|(i, mut f)| {
            f.id = format!("FND-{:06}", i + 1);
            f
        })
        .collect()
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn malformed() {
        assert!(parse(b"invalid yaml", "v", "raw/x", Path::new(".")).is_err());
        let located = b"Diagnostics:\n  - {DiagnosticName: misra-x, DiagnosticMessage: {Message: m, FilePath: '', FileOffset: 0}}\n";
        let err = parse(located, "v", "raw/x", Path::new("."))
            .unwrap_err()
            .to_string();
        assert!(err.contains("misra-x has no source location"), "{err}");
    }
    #[test]
    fn correlate_merges_only_identical_diagnostics() {
        let finding = |message: &str, reference: &str| {
            serde_json::from_value::<Finding>(serde_json::json!({"id":"F","tool":"clang-tidy","rule_id":"misra-x","file":"h.h","line":1,"column":1,"severity":"warning","message":message,"build_variant":"VAR-1","outcome":"diagnostic","category":"static-diagnostic","native_references":[reference]})).unwrap()
        };
        let merged = correlate(
            vec![finding("same", "raw/a.log"), finding("same", "raw/b.log")],
            Path::new("/"),
        );
        assert_eq!(merged.len(), 1);
        assert_eq!(merged[0].native_references, ["raw/a.log", "raw/b.log"]);
        let distinct = correlate(
            vec![
                finding("first claim", "raw/a.log"),
                finding("second claim", "raw/b.log"),
            ],
            Path::new("/"),
        );
        assert_eq!(distinct.len(), 2);
    }
    #[test]
    fn positions() {
        let d = tempfile::tempdir().unwrap();
        std::fs::write(d.path().join("x.c"), "ab\ncd\n\nef").unwrap();
        let export = "Diagnostics:\n\
            - {DiagnosticName: a, Level: Warning, DiagnosticMessage: {Message: m, FilePath: x.c, FileOffset: 0, Ranges: [{FilePath: x.c, FileOffset: 0, Length: 4}]}}\n\
            - {DiagnosticName: b, DiagnosticMessage: {Message: m, FilePath: x.c, FileOffset: 2}}\n\
            - {DiagnosticName: c, DiagnosticMessage: {Message: m, FilePath: x.c, FileOffset: 7}}\n\
            - {DiagnosticName: d, DiagnosticMessage: {Message: m, FilePath: x.c, FileOffset: 9}}\n";
        let f = parse(export.as_bytes(), "v", "raw/x", d.path()).unwrap();
        let at: Vec<_> = f.iter().map(|f| (f.line, f.column)).collect();
        assert_eq!(at, [(1, 1), (1, 3), (4, 1), (4, 3)]);
        assert_eq!((f[0].end_line, f[0].end_column), (Some(2), Some(2)));
        assert_eq!(f[1].end_line, None);
        assert!(parse(export.replace("9}}", "10}}").as_bytes(), "v", "r", d.path()).is_err());
    }
    #[test]
    fn empty() {
        assert!(
            parse(b"Diagnostics: []", "v", "raw/x", Path::new("."))
                .unwrap()
                .is_empty()
        );
    }
}
