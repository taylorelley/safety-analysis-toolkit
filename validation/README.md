# Executed development validation

These are actual outputs, not sample success records. This is not a v1 release
or a certification/compliance statement.

- `full-validation.log`: format, Clippy, release build, 21 unit tests, 12
  integration tests, two native CTest suites, JSON/SARIF validation, real ESBMC
  C/C++ assertions and insufficient-bound fixtures. All available suites passed.
- `e2e/summary.json`: seven CLI operations and six independent MCP operations.
  The complete assurance workflow intentionally remains **incomplete**.
- `e2e/consolidated-report.md`: persisted historical operation scopes, defects,
  scoped proofs, counterexamples, trace gaps, measured MC/DC gaps and limitations.
- `e2e/runs/*/`: manifests, native output, config snapshots, normalized results,
  counterexamples, AST records, coverage exports and per-run SHA inventories.
  Completed records are copied without overwriting an existing archive.
- `exploratory-demo/constant-caller-scan.json`: retained initial scope limitation;
  the fixed caller made the null path invisible to the selected Clang analysis.
  It is not represented as a successful defect-detection benchmark.

Reference implementation commit for the final execution: `1c383de`.
Earlier coherent slices: `ae6d42a` (foundation/Clang/native pilot), `51cf372`
(AST/ESBMC/coverage/MCP/skill), `d5f1a75` (variant/failure safeguards), `6a14068`
(scoped report rendering). Each run also records the project Git metadata,
dirty/untracked input hashes, actual toolkit executable identity and analyzer
commands/versions/hashes. Earlier runs preserve their own earlier source scope;
they are not silently merged as current evidence.

Frama-C Eva/WP was **not run**. Its request records NOT_CHECKED/incomplete.
No MISRA guideline mapping is validated. The independent client exercises actual
MCP stdio, but Claude Code/Codex/Hermes/Pi themselves have not been exercised.
Successful Landlock enforcement could not be tested on this host; strict mode's
failure and network denial were tested. Full release acceptance remains as
listed in `docs/development-status.md`.

`SHA256SUMS` covers archived validation files except itself. These hashes detect
changes, not an attacker able to replace both artifacts and inventories. Native
per-run integrity checks are the authoritative toolkit checks. Analyzer binaries,
third-party bundles, coverage executables and proprietary MISRA text are not
included in this archive.
