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
the evidence directory and invokes pinned clang-tidy with explicit checks,
explicit configuration and YAML diagnostic export. Project `.clang-tidy`
configuration is overridden. Static Analyzer diagnostics arrive through
`clang-analyzer-*`. The runner uses direct argv execution, no shell. The
compiler named in the database is never executed.

LLVM 19.1.7 emits no export file on a diagnostic-free run. That case is accepted
only with successful exit and empty stdout/stderr. Malformed structured output,
unexpected output without an export, parser errors and timeouts are incomplete.
Raw output is always retained. A diagnostic remains a diagnostic, not a formally
established production defect. Correlation preserves property and variant.

The custom C++ module uses Clang AST matchers. Its `goto` policy check has no
MISRA rule identifier: licensed-standard validation is outstanding. CMake
requires LLVM 19.1.7 exactly. Native module loading is not yet exposed by the
Rust CLI; CTest validates it directly against the matching clang-tidy host.

Run records include input content hashes, exact execution/environment records,
executable hashes, available Git commit/dirty metadata, versions and per-unit
completeness. Input changes during a run force incomplete status. Headers and
other dependencies outside the workspace are not fingerprinted yet. No cached
results are reused. Reports operate only on integrity-checked persisted records.
