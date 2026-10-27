# Architecture

One Rust binary contains config validation, discovery, a shared runner,
diagnostic normalization, evidence storage and reporting. CLI operations call
these modules directly. There is no service, database, build-system execution,
cache, LLM or network dependency during analysis.

`scan` loads reviewed `safety.yaml`, parses the compilation database, keeps each
unique file/directory/compiler/argument combination as a separate build variant,
and rejects flags outside a deliberately small compatibility vocabulary. It
never translates or silently discards unknown semantic options. Discovery
records includes, definitions, dialect, target and compiler extensions alongside
the exact original argument vector.

For each supported unit the engine writes a dedicated compilation database in
the evidence directory and invokes clang-tidy 19.1.x with explicit checks,
explicit configuration and YAML diagnostic export. Project `.clang-tidy`
configuration is overridden. Static Analyzer diagnostics arrive through
`clang-analyzer-*`. The runner uses direct argv execution, no shell. The
compiler named in the database is never executed.

LLVM 19.1 emits no export file on a diagnostic-free run. That case is accepted
only with successful exit and empty stdout/stderr. Malformed structured output,
unexpected output without an export, parser errors and timeouts are incomplete.
Raw output is always retained. A diagnostic remains a diagnostic, not a formally
established production defect. Correlation preserves property and variant.

The custom C++ module uses Clang AST matchers, preprocessor callbacks and
comment handlers. Its three original policy checks carry no MISRA rule
identifier. Its `misra-c2012-*` checks implement partial MISRA C:2012 guideline
mappings that await independent review (see [MISRA coverage](misra-coverage.md)). CMake
requires LLVM 19.1.0 or a later 19.1.x patch release; build against the same LLVM as the clang-tidy host. Native module loading requires an explicit reviewed module path and allowlist.
CTest and a real CLI fixture validate loading against the matching host.

Run records include input content hashes, exact execution/environment records,
executable hashes, available Git commit/dirty metadata, versions and per-unit
completeness. Input changes during a run force incomplete status. Headers and
other dependencies outside the workspace are not fingerprinted yet. No cached
results are reused. Reports operate only on integrity-checked persisted records.

AST traceability, Frama-C, ESBMC and LLVM coverage adapters use the same runner and evidence store. MCP uses rmcp stdio and calls those same operations with one fixed workspace, bounded results and cancellation. See their dedicated references for current limitations.


Formal requests are validated by one verification module before adapters run.
Harness metadata is a separate input artifact, never an approval. Runner output
is retained byte-for-byte within its capture bound; manifest excerpts are capped
at 16 KiB per stream and point to raw files. An output limit produces incomplete
execution. Finding records preserve property/rule, build context, source range,
certainty where meaningful and requirement/test/disposition references.

MISRA edition registries, deviation/GRP drafts and externally supplied human
reviews remain distinct from analyzer results. Validating their format cannot
authenticate a review or infer guideline compliance. No database or cache is used.
