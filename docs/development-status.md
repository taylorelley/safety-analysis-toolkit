# Implementation status

This is a working development snapshot; **the v1 definition of done is not met**.

| Phase | Executed functionality | Outstanding acceptance |
| --- | --- | --- |
| 1 Foundation | Config, CLI, compilation databases/variants, restricted runner, tools, Git/input provenance, evidence | Wider compiler/platform support and complete external dependency fingerprints |
| 2 Clang | Real C/C++ analysis, native diagnostics, source references, per-unit completeness, correlation, SARIF | Larger independent benchmark corpus |
| 3 MISRA technology | Pinned module, direct-goto project check, real fixtures/CLI loading, incomplete GEP/GCS drafts | Licensed guideline inventory, validated mappings, edition registries, deviations/GRP |
| 4 Traceability | Clang USRs, declarations/definitions, comments/templates/overloads, manifest graph and gap checking | Call edges, comprehensive specialization/annotation/build-variant reconciliation |
| 5 Formal | ESBMC 8.6.0 C/C++ safe/violated/insufficient-bound tests, model scopes, bounds, machine counterexamples | Frama-C Eva/WP, reviewed harness metadata, induction, broader property taxonomy |
| 6 Coverage/evidence | Real LLVM source/MC/DC import, known 50% gap, provenance checks, persisted consolidated reports | Full instrumentation/exclusion assessment and requirement/test graph joins |
| 7 MCP/skill | SDK-backed eight-tool stdio server, strict inputs, bounded results, cancellation, reusable skill, real independent client | Two actual agent harnesses and complete end-to-end agent review |
| 8 Packaging | Build/check targets, lockfile, CI definition, examples, dependency inventory, validation records | Fresh-machine CI execution, release license review, packaging and complete v1 validation |

Filesystem isolation uses Landlock where available. Its absence is explicitly
recorded; requiring it fails closed. This host lacks Landlock, so no successful
Landlock enforcement validation is claimed here. Seccomp networking denial,
process timeout/cancellation and read-only analyzer invocation are tested.

The user authorized unmapped custom checks while licensed MISRA validation is
unavailable. No exact rule IDs are selected or inferred. Frama-C requests record
NOT_CHECKED/incomplete after capability inspection; Eva/WP is not presented as an
implemented adapter. No phase is declared complete solely from mocked outputs.

See [validation](validation.md), [formal](formal-verification.md),
[traceability](traceability.md), [coverage](coverage.md) and [MCP](mcp.md).
