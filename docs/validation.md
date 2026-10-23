# Development validation log

## Foundation and real Clang slice

Implemented: strict config and path validation, CLI init/doctor/scan/runs/report,
compilation-unit and build-variant discovery, conservative flag compatibility,
shared runner, network denial, timeout/cancellation/resource/output bounds,
raw and normalized diagnostics, evidence provenance and integrity checks,
Markdown/JSON/SARIF exports, C/C++ examples.

Executed during development:

- `cargo test`: foundation, runner, integrity and CLI failure tests passed.
- `cargo test -- --include-ignored` with LLVM 19.1.7: real C and C++ scans,
  safe fixtures, distinct variants and parser-failure paths passed.
- `cargo clippy --all-targets -- -D warnings` passed.
- `cargo build --release --locked` passed.
- `python3 scripts/validate.py`: both examples analyzed 4/4 units, each finding
  the two expected variant-specific null dereferences. JSON schemas, official
  SARIF schema, report regeneration and SHA inventories passed.

The test counts and final run transcripts are recorded in the validation
artifacts committed after the implementation commit. Initial exploratory failed
runs are retained locally, not represented as successful verification.

## Native check pilot

Executed CMake configure/build against Debian LLVM 19.1.7, and CTest. One CTest
case executed six real clang-tidy invocations: positive, negative, boundary,
macro, C++ template/overload and invalid-source fixtures. All six passed.
No MISRA rule mapping was tested or claimed.

## Known environment limitation

Both the task container and native nested Docker runtime return ENOSYS for
Landlock. Seccomp socket-denial was verified with a real Python socket call.
Strict filesystem-isolation mode refuses execution. The fallback explicitly
records absence of filesystem isolation, uses read-only analyzer operations,
and verifies source fingerprints. It is not described as a filesystem sandbox.
Local LLVM packages were downloaded from the environment's Debian snapshot and
extracted under `/workspace/scratch/llvm`; clang-tidy's RPATH was adjusted to
load these libraries without inheriting LD_LIBRARY_PATH in analyzer children.
This installation is development setup, not a distributed binary or installer.

## Remaining acceptance

No formal, traceability, MC/DC, whole-agent harness, MISRA-compliance or complete
v1 validation has been executed. Those acceptance criteria remain outstanding.
The next vertical slice is AST indexing and graph validation, alongside broader
unmapped custom checks and reviewed MISRA enforcement metadata.

## Subsequent integrated slice

Implemented: Clang AST source indexer and graph-gap checks; scoped ESBMC adapter
with native property classification and counterexample JSON; LLVM source/MC/DC
import with provenance checks; eight-tool rmcp stdio server; incomplete MISRA
GEP/GCS drafts; reusable agent skill; consolidated persisted reports.

Latest combined real suite: 21 unit tests and 12 integration tests passed,
including custom native-module loading, C/C++ static scans, AST trace, independent
MCP client, LLVM 50% MC/DC gap/corrupt-profile tests and six ESBMC C/C++ property
fixtures. CTest also passed both native suites. Ordinary Cargo runs explicitly
ignore optional external-tool tests; `--include-ignored` was used for these
results with all available tools configured.

An earlier full run caught a cancellation regression in post-execution hashing;
executable hashing now happens before spawn and the real cancellation test
passes. An earlier trace run caught missing default allowlist registration; the
indexer is now registered and its real CLI test passes. These failed runs were
not classified as validation success.

Formal limitations: the ESBMC tests are real, but Frama-C Eva/WP has not executed.
MISRA mappings remain unvalidated by explicit user direction. Traceability call
edges/specialization completeness, reviewed harness metadata, all instrumentation
exclusions and two actual coding-agent harnesses remain acceptance gaps.

The mixed-language demonstration is prepared explicitly with
`python3 scripts/configure_demo.py` (this builds and executes its development
coverage fixture), then `python3 scripts/e2e.py`. The latter records CLI and
independent MCP outputs and copies integrity-checked run artifacts into
`validation/e2e/`. The report deliberately remains incomplete. This client is
not passed off as Claude Code, Codex, Hermes or Pi.

The first mixed-demo scan found the custom check but not the library's null
path when its only visible caller passed constant one. That result is preserved
in `validation/exploratory-demo/constant-caller-scan.json`. The demonstration now
keeps the open-input library translation unit separate from its fixed-input
verification harness. This is a real Clang interprocedural-scope limitation, not
a claim that scanning a TU explores every possible function context. The
standalone library scan detects the null path; the harness proves only its
explicit fixed-input property. No broad proof is inferred from that harness.

Final executed source commit: `1c383de` (toolkit identity provenance), preceded by
`6a14068` (scoped reports), `d5f1a75` (failure/variant safeguards), `51cf372`
(AST/ESBMC/coverage/MCP/skill) and `ae6d42a` (foundation and Clang pipeline).
`validation/full-validation.log` records the final available-tool run. The
mixed example also ran through native CLI and the independent MCP client, with
its native records archived and verified. No actual agent-harness acceptance or
Frama-C result is claimed. Next work is the outstanding acceptance items in the
phase table, especially a validated Frama-C adapter and reviewed MISRA metadata.
