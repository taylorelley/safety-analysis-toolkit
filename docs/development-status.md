# Implemented scope and acceptance boundaries

The native toolkit, CLI, MCP, original Clang checks, formal adapters,
traceability, coverage/evidence workflow and release tooling are implemented.
The MISRA C:2012 guideline inventory (identifiers and classifications from a
user-supplied licensed copy) is complete. At the user's later request, 157
guidelines have **partial** automated checks with known-answer fixtures; Dir 3.1
and Rule 8.6 remain manual. The mappings are reviewed only by their
implementation author and need independent review.

| Phase | Working implementation and real validation | Capability boundary |
| --- | --- | --- |
| 1 Foundation | Strict config/path validation, init/doctor, compilation variants, one runner, versions, provenance and bounded JSON | Linux x86-64 is the validated reference; proprietary compiler flags can be unsupported |
| 2 Clang | C/C++ clang-tidy/static analyzer, normalized native ranges, per-unit completeness, SARIF and seven-case benchmark | Selected checks do not find every defect; no unvalidated result cache |
| 3 Original checks/MISRA process | Three original policy checks, 148 MISRA C:2012 guideline checks with 164 known-answer fixtures, edition registries, reviewed-mapping input validation, draft GEP/GRP/GCS/deviations and separate human-record import | MISRA C:2012 mappings are partial and await independent review; the MISRA C++:2023 inventory is empty; no compliance/approval grant |
| 4 Traceability | AST USRs, declarations/definitions, direct calls, templates/specializations, explicit per-build graphs, cycles/unknown/untraced/baseline gaps | Dynamic/virtual/indirect calls and ambiguous ownership remain visible investigation gaps |
| 5 Formal | Real Frama-C Eva/WP and ESBMC C/C++ BMC/induction, validated harness metadata, property outcomes, scoped assumptions/bounds and native counterexamples | Sequential source/library models; user assumptions need authorized review; no unrestricted bounded-proof claim |
| 6 Coverage/evidence | Native LLVM source/branch/MC/DC import, per-condition gaps, provenance and trace joins, immutable run creation, SHA checks and persisted reports | Host/target provenance is supplied; instrumentation exclusions, coupling/object/target/qualification evidence remain separate objectives |
| 7 MCP/skill | Maintained SDK stdio, all eight tools, strict schemas, bounded pages, timeout/cancellation, real independent client and actual Codex/Pi workflows; reusable skill references upstream lifecycle policy | Host transport/workflow tests use no model inference and do not establish agent reasoning or independent review; Claude/Hermes configs are examples |
| 8 Packaging/validation | Locked builds, CI jobs, isolated offline reference validation, original-artifact installer/archive, dependency notices and executed validation records | External analyzers are not bundled; remote GitHub Actions has not been run from this local workspace |

`complete` is specific to the selected operation/configuration. The intentionally
unsafe mixed demonstration retains defects, partial MISRA mappings, missing
trace relationships and a known MC/DC gap. Its consolidated result must remain
incomplete. Software implementation acceptance does not close those project
assurance objectives.

Landlock is used when available; this host lacks it. Strict isolation fails
closed and no successful Landlock-enforcement claim is made for this host.
Seccomp network denial, process resource/time/output limits, MCP cancellation
and CLI SIGINT are exercised. External toolchain/runtime reproduction requires
the recorded reference environment; source/profile/binary inputs are archived
for the demo. There is no certification, authentication or tamper-proof claim.

See [executed validation](validation.md), [installation](installation.md),
[formal scope](formal-verification.md), [traceability](traceability.md),
[coverage](coverage.md) and [MCP](mcp.md).
