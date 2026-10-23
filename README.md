# Safety Analysis Toolkit

A local Rust CLI for evidence-preserving C/C++ analysis. This development
snapshot implements the executable foundation and real Clang Static Analyzer /
Clang-Tidy scans. A tested out-of-tree Clang-Tidy module provides an original
project-policy `goto` check. It has **no validated MISRA mapping**.

This is not the completed v1 specification. It also provides AST trace checks,
scoped ESBMC verification, LLVM source/MC/DC import and an SDK-backed MCP stdio
server. Frama-C execution, validated MISRA mappings, complete process artifacts
and two-agent-harness acceptance remain outstanding. See [development status](docs/development-status.md).

## Build and use

Linux x86-64 or AArch64, Rust 1.99.1, LLVM/Clang **19.1.7**. Install `clang-tidy-19`,
`clang-19`, `llvm-19-dev`, `libclang-19-dev`, CMake >=3.25, a C++17 compiler and
Python 3. See [contributing](CONTRIBUTING.md) for validation.

```sh
cargo build --locked --release
export PATH="$PWD/target/release:$PATH"
safety init --path /path/to/project
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
