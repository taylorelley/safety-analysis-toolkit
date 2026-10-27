// External tools are gated on a minimum version, with LLVM limited to 19.1.x.
// Other supported releases carry an explicit validation limitation.
pub const LLVM_MINIMUM: &str = "19.1.0";
pub const LLVM_REFERENCE: &str = "19.1.7";
pub const ESBMC_MINIMUM: &str = "8.6.0";
pub const ESBMC_REFERENCE: &str = "8.6.0";
pub const FRAMA_C_MINIMUM: &str = "25.0";
pub const FRAMA_C_REFERENCE: &str = "25.0-beta";

#[derive(Debug, PartialEq, Eq)]
pub struct Version {
    numbers: Vec<u64>,
    release: String,
}

/// Release token immediately following `marker`, retaining its numeric prefix.
pub fn parse(text: &str, marker: &str) -> Option<Version> {
    let rest = &text[text.find(marker)? + marker.len()..];
    let digits = rest
        .split(|c: char| !c.is_ascii_digit() && c != '.')
        .next()?
        .trim_end_matches('.');
    let parts = digits
        .split('.')
        .map(|p| p.parse().ok())
        .collect::<Option<Vec<u64>>>()?;
    (!parts.is_empty()).then(|| Version {
        numbers: parts,
        release: rest.split_whitespace().next().unwrap().to_string(),
    })
}
fn numbers(v: &str) -> Vec<u64> {
    parse(v, "").map(|v| v.numbers).unwrap_or_default()
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
pub fn display(v: &Version) -> String {
    v.numbers
        .iter()
        .map(u64::to_string)
        .collect::<Vec<_>>()
        .join(".")
}
/// Ok(limitation) when `found` meets `minimum`; the limitation is set when it
/// differs from the validated reference. LLVM must also be in the 19.1 series.
/// Frama-C reference matching includes the release suffix. Err when unsupported.
pub fn check(
    tool: &str,
    found: Option<Version>,
    minimum: &str,
    reference: &str,
) -> Result<Option<String>, String> {
    let Some(found) = found else {
        return Err(format!(
            "{tool} version unrecognised; requires {minimum} or later"
        ));
    };
    if compare(&found.numbers, &numbers(minimum)).is_lt() {
        return Err(format!(
            "{tool} {} is older than the minimum supported {minimum}",
            found.release
        ));
    }
    if matches!(tool, "LLVM" | "Coverage import LLVM") && !found.numbers.starts_with(&[19, 1]) {
        return Err(format!(
            "{tool} {} is outside the supported LLVM 19.1.x series",
            found.release
        ));
    }
    let differs = if tool == "Frama-C" {
        found.release != reference
    } else {
        compare(&found.numbers, &numbers(reference)).is_ne()
    };
    Ok(differs.then(|| {
        format!(
            "{tool} {} differs from the validated reference {reference}; output interpretation for this release is unvalidated",
            found.release
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
            )
            .map(|v| v.numbers),
            Some(vec![19, 1, 1])
        );
        assert_eq!(
            parse("ESBMC version 8.6.0 64-bit", "ESBMC version ").map(|v| v.numbers),
            Some(vec![8, 6, 0])
        );
        let frama = parse("25.0-beta (Manganese)", "").unwrap();
        assert_eq!(frama.numbers, vec![25, 0]);
        assert_eq!(frama.release, "25.0-beta");
        assert_eq!(parse("29.0 (Copper)", "").unwrap().numbers, vec![29, 0]);
        assert_eq!(parse("version unknown", "version "), None);
        assert_eq!(parse("no marker", "LLVM version "), None);
    }
    #[test]
    fn minimum_and_reference() {
        let llvm = |v| check("LLVM", parse(v, ""), LLVM_MINIMUM, LLVM_REFERENCE);
        assert_eq!(llvm("19.1.7"), Ok(None));
        assert!(llvm("19.1.1").unwrap().is_some());
        assert!(llvm("19.1.0").unwrap().is_some());
        assert!(llvm("18.1.8").is_err());
        assert!(check("ESBMC", None, ESBMC_MINIMUM, ESBMC_REFERENCE).is_err());
        assert!(check("ESBMC", parse("8.5.9", ""), ESBMC_MINIMUM, ESBMC_REFERENCE).is_err());
        assert_eq!(
            check("ESBMC", parse("8.6.0", ""), ESBMC_MINIMUM, ESBMC_REFERENCE),
            Ok(None)
        );
        assert!(
            check("ESBMC", parse("9.0.0", ""), ESBMC_MINIMUM, ESBMC_REFERENCE)
                .unwrap()
                .is_some()
        );
    }
    #[test]
    fn llvm_requires_the_19_1_series_for_scan_and_coverage() {
        for tool in ["LLVM", "Coverage import LLVM"] {
            let check_llvm = |v| check(tool, parse(v, ""), LLVM_MINIMUM, LLVM_REFERENCE);
            for release in ["19.0.9", "19.2.0", "20.0.0", "version unknown"] {
                assert!(check_llvm(release).is_err(), "{tool}: {release}");
            }
            assert_eq!(check_llvm("19.1.7"), Ok(None));
            assert!(check_llvm("19.1.0").unwrap().is_some());
            assert!(check_llvm("19.1.8").unwrap().is_some());
        }
    }
    #[test]
    fn frama_reference_includes_the_release_suffix() {
        let frama = |v| check("Frama-C", parse(v, ""), FRAMA_C_MINIMUM, FRAMA_C_REFERENCE);
        assert_eq!(frama("25.0-beta (Manganese)"), Ok(None));
        for release in ["25.0 (Manganese)", "25.0-rc1 (Manganese)", "26.1 (Iron)"] {
            let note = frama(release).unwrap().unwrap();
            assert!(note.contains(release.split_whitespace().next().unwrap()));
            assert!(note.contains(FRAMA_C_REFERENCE));
        }
        assert!(frama("24.0 (Chromium)").is_err());
        assert!(frama("version unknown").is_err());
    }
}
