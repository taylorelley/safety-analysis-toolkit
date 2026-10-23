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
