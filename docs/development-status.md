# Implementation status

This repository is a working development snapshot, not v1 complete.

- Foundation: configuration/CLI, compilation databases and variant discovery,
  restricted shared runner, tool probes, source hashes, execution provenance,
  Git metadata and integrity-checked filesystem evidence work.
- Clang: real C/C++ Static Analyzer/Tidy scans, native diagnostic preservation,
  source locations, per-unit completeness, correlation, Markdown/JSON/SARIF
  export work. Current compatibility vocabulary intentionally rejects many flags.
- Native module: LLVM 19.1.7 module and original `safety-prohibited-goto` check
  work on six real fixtures. MISRA rule mappings remain unvalidated, as directed
  by the user. No MISRA guideline coverage is claimed. GEP/GRP/GCS and deviation
  management are not implemented.
- MCP: SDK-backed stdio interface provides discovery, scan, findings, run lookup
  and reporting against one configured workspace. Real-client validation is
  recorded separately; harness-specific end-to-end acceptance remains pending.
- AST traceability, Frama-C/Eva/WP, ESBMC, LLVM coverage/MC/DC import, full
  assurance reports and release packaging remain unimplemented.

There are no placeholder `prove`, `trace` or `coverage` commands returning
success. They and their MCP tools will be added with real adapters and fixtures.
The unimplemented phases prevent claiming the overall v1 definition of done.

Design changes: filesystem write isolation is used when Landlock is available;
its absence is recorded, with an opt-in requirement that fails closed. This
preserves useful trusted-tool scans on the current host, which has no Landlock.
No successful Landlock enforcement test result is claimed on this host.
The user authorized unmapped custom checks while licensed-standard mappings
remain unvalidated. No exact MISRA IDs have been selected or inferred.

See [validation log](validation.md) for executed checks and commit provenance.
