// External tools are gated on a minimum version. Releases other than the
// validated reference are accepted but recorded as an explicit limitation.
pub const LLVM_MINIMUM: &str = "19.1.0";
pub const LLVM_REFERENCE: &str = "19.1.7";
pub const ESBMC_MINIMUM: &str = "8.6.0";
pub const ESBMC_REFERENCE: &str = "8.6.0";
pub const FRAMA_C_MINIMUM: &str = "25.0";
pub const FRAMA_C_REFERENCE: &str = "25.0";

/// Dotted numeric version immediately following `marker` in `text`.
pub fn parse(text: &str, marker: &str) -> Option<Vec<u64>> {
    let rest = &text[text.find(marker)? + marker.len()..];
    let digits = rest
        .split(|c: char| !c.is_ascii_digit() && c != '.')
        .next()?
        .trim_end_matches('.');
    let parts = digits
        .split('.')
        .map(|p| p.parse().ok())
        .collect::<Option<Vec<u64>>>()?;
    (!parts.is_empty()).then_some(parts)
}
fn numbers(v: &str) -> Vec<u64> {
    v.split('.').filter_map(|p| p.parse().ok()).collect()
}
fn compare(a: &[u64], b: &[u64]) -> std::cmp::Ordering {
    let n = a.len().max(b.len());
    let pad = |v: &[u64]| {
        (0..n)
            .map(|i| v.get(i).copied().unwrap_or(0))
            .collect::<Vec<_>>()
    };
    pad(a).cmp(&pad(b))
}
pub fn display(v: &[u64]) -> String {
    v.iter().map(u64::to_string).collect::<Vec<_>>().join(".")
}
/// Ok(limitation) when `found` meets `minimum`; the limitation is set when it
/// differs from the validated reference. Err(reason) when unsupported.
pub fn check(
    tool: &str,
    found: Option<Vec<u64>>,
    minimum: &str,
    reference: &str,
) -> Result<Option<String>, String> {
    let Some(found) = found else {
        return Err(format!(
            "{tool} version unrecognised; requires {minimum} or later"
        ));
    };
    if compare(&found, &numbers(minimum)).is_lt() {
        return Err(format!(
            "{tool} {} is older than the minimum supported {minimum}",
            display(&found)
        ));
    }
    Ok(compare(&found, &numbers(reference)).is_ne().then(|| {
        format!(
            "{tool} {} differs from the validated reference {reference}; output interpretation for this release is unvalidated",
            display(&found)
        )
    }))
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn parses_tool_banners() {
        assert_eq!(
            parse(
                "Ubuntu LLVM version 19.1.1\n  Optimized build.",
                "LLVM version "
            ),
            Some(vec![19, 1, 1])
        );
        assert_eq!(
            parse("ESBMC version 8.6.0 64-bit", "ESBMC version "),
            Some(vec![8, 6, 0])
        );
        assert_eq!(parse("25.0-beta (Manganese)", ""), Some(vec![25, 0]));
        assert_eq!(parse("29.0 (Copper)", ""), Some(vec![29, 0]));
        assert_eq!(parse("version unknown", "version "), None);
        assert_eq!(parse("no marker", "LLVM version "), None);
    }
    #[test]
    fn minimum_and_reference() {
        let llvm = |v| check("LLVM", Some(v), LLVM_MINIMUM, LLVM_REFERENCE);
        assert_eq!(llvm(vec![19, 1, 7]), Ok(None));
        assert!(llvm(vec![19, 1, 1]).unwrap().is_some());
        assert!(llvm(vec![19, 1, 0]).unwrap().is_some());
        assert!(llvm(vec![18, 1, 8]).is_err());
        assert!(check("ESBMC", None, ESBMC_MINIMUM, ESBMC_REFERENCE).is_err());
        assert!(check("ESBMC", Some(vec![8, 5, 9]), ESBMC_MINIMUM, ESBMC_REFERENCE).is_err());
        assert_eq!(
            check(
                "Frama-C",
                Some(vec![25, 0]),
                FRAMA_C_MINIMUM,
                FRAMA_C_REFERENCE
            ),
            Ok(None)
        );
        assert!(
            check(
                "Frama-C",
                Some(vec![26, 1]),
                FRAMA_C_MINIMUM,
                FRAMA_C_REFERENCE
            )
            .unwrap()
            .is_some()
        );
    }
}
