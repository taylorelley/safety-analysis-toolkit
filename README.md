# Safety Analysis Toolkit

A local Rust CLI and MCP server for C/C++ static analysis, scoped formal
verification, AST traceability, LLVM source/MC/DC coverage and reproducible evidence.
It uses Clang/LLVM 19.1 (19.1.0 or later), Frama-C Eva/WP and ESBMC rather than new analysis engines.
The out-of-tree Clang-Tidy module implements three original policy checks and
148 MISRA C:2012 guideline checks. **157 of 159 MISRA C:2012 guidelines have
partial automated assessment; no mapping is independently reviewed and no
compliance is claimed** (see [MISRA coverage](docs/misra-coverage.md)).

The same engine serves humans and agents. Real Codex and Pi host workflows,
native integrations, packaging and offline reference validation are recorded in
[validation](docs/validation.md). See [capability boundaries](docs/development-status.md).

## Build and use

Quickest routes: `scripts/bootstrap.sh` installs every dependency (LLVM/Clang,
Rust, Frama-C with Eva and WP, ESBMC), builds, tests and installs the toolkit;
`make image` builds a container that serves MCP on start. See
[installation](docs/installation.md).

Minimum versions: Rust 1.89, LLVM/Clang **19.1.0** (or a later 19.1.x), ESBMC 8.6.0 and
Frama-C 25.0. Validated reference: Linux x86-64, Rust 1.99.1, LLVM/Clang 19.1.7.
Runs with a supported release other than the validated reference record that
output interpretation for that release is unvalidated.
AArch64 runner code exists but has not been validated here. Install `clang-tidy-19`,
`clang-19`, `llvm-19-dev`, `libclang-19-dev`, `libclang-rt-19-dev`, CMake >=3.25, a C++17 compiler and
Python 3. See [contributing](CONTRIBUTING.md) for validation.

```sh
cargo build --locked --release
export PATH="$PWD/target/release:$PATH"
safety init --path /path/to/project
# Or enable the MISRA/policy module; this writes an explicit allowlist of the
# module and supported tools resolved from PATH. Review it before use.
safety init --path /path/to/project --module "$PWD/build/clang/SafetyMisra.so"
# Supply an existing compile_commands.json; safety never runs a project build.
safety doctor /path/to/project
safety scan /path/to/project --format json
safety trace /path/to/project
safety mcp --path /path/to/project
safety runs /path/to/project
safety report /path/to/project --run RUN-ID --format markdown
```

To use the bundled known-answer examples:

```sh
python3 examples/configure.py
safety doctor examples/embedded-c
safety scan examples/embedded-c
safety scan examples/embedded-cpp
```

Both examples deliberately contain a null dereference. Their scans should exit
1 with two findings, one for each build variant, and four analyzed units.
The safe functions should produce no findings.

All dependencies are external. After installing dependencies and caching Cargo
packages, `cargo build --locked --offline` and analysis need no network.
The engine denies analyzer socket creation, clears the child environment,
bounds output, enforces time/memory limits and rejects unreviewed compiler
flags. It never executes the compiler named in a compilation database, project
build scripts, or test programs. No automatic fixes are requested.

Scans analyse one translation unit at a time by default. Set `tools.jobs`
(1 to 64) to run that many Clang-Tidy processes concurrently; each keeps its own
time and memory limits, so peak memory scales with `jobs`. Findings and
recorded executions keep compilation database order regardless of scheduling.

Analyzer writes are restricted with Landlock when available. When unavailable,
executions explicitly record the absence of filesystem isolation. Set
`tools.require_filesystem_isolation: true` to refuse execution without it.
This toolkit is not a general sandbox; see [limitations](docs/assurance-limitations.md).
Project configuration and executable resolution must be reviewed before use.

## Evidence and interpretation

Every scan writes a unique `.safety/runs/RUN-...` directory containing execution
provenance, captured diagnostics, structured findings, a Markdown report and a
SHA-256 inventory. Reports verify stored hashes before loading results. Files
are not tamper-proof: someone able to rewrite evidence and its inventory can
replace both. No signing or independent attestation is claimed.

`complete` describes execution of the **selected operation and configured
checks**. It does not establish complete defect detection, MISRA compliance,
verification completeness, independent review, or certification. Missing tools,
unsupported compiler flags and failed units produce an explicit incomplete
status and exit 3. Assurance levels default to unconfigured.

Original code is Apache-2.0. MISRA standard text is not included. See
[dependency inventory](docs/dependencies.md) before redistributing binaries. See [formal verification](docs/formal-verification.md),
[traceability](docs/traceability.md), [coverage](docs/coverage.md) and
[MCP](docs/mcp.md) for capability boundaries.

Executed results: [mixed-project report](validation/completion/e2e/consolidated-report.md),
[agent-host workflows](validation/completion/harnesses/summary.json), and
[offline reference validation](validation/completion/reference-validation.log).
These are executed tool workflows, not independent human review or an automated
claim of certification. Installation and release commands are in
[packaging](docs/installation.md).
