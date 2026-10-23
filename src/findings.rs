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
}
pub fn parse(
    data: &[u8],
    variant: &str,
    reference: &str,
    directory: &Path,
) -> Result<Vec<Finding>> {
    let export: Export =
        serde_yaml::from_slice(data).context("invalid clang diagnostics export")?;
    let mut findings = Vec::new();
    for d in export.diagnostics {
        let file = directory.join(&d.message.file);
        let source = std::fs::read(&file).context("diagnostic source unavailable")?;
        let offset = d.message.offset.min(source.len());
        let prefix = &source[..offset];
        let line = prefix.iter().filter(|&&b| b == b'\n').count() + 1;
        let column = prefix
            .iter()
            .rposition(|&b| b == b'\n')
            .map_or(offset + 1, |n| offset - n);
        findings.push(Finding {
            id: String::new(),
            tool: "clang-tidy-19".into(),
            rule_id: d.name,
            file: file.display().to_string(),
            line,
            column,
            severity: d.level.unwrap_or("warning".into()).to_lowercase(),
            message: d.message.text,
            build_variant: variant.into(),
            outcome: "DIAGNOSTIC".into(),
            native_references: vec![reference.into()],
        });
    }
    Ok(findings)
}
pub fn correlate(findings: Vec<Finding>, root: &Path) -> Vec<Finding> {
    let mut unique: BTreeMap<(String, usize, usize, String, String), Finding> = BTreeMap::new();
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
