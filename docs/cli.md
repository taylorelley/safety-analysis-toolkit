# CLI reference

This snapshot supports these operations:

| Command | Arguments | Result |
| --- | --- | --- |
| `init` | `--path DIR` (default `.`) | Create config without overwriting |
| `doctor` | `[DIR] --format json/markdown/sarif` | Tool probes and unit inventory |
| `trace` | `[DIR] --format json/markdown/sarif` | AST relationship audit |
| `prove` | `FILE --path DIR --engine esbmc/frama-c --variant ID --method bmc/induction/eva/wp --entry main --harness FILE --unwind N --bits 32/64` | Scoped properties or explicit unsupported status |
| `coverage import` | `--path DIR --profile FILE --binary FILE [--provenance FILE]` | Existing LLVM coverage import |
| `mcp` | `--path DIR` | SDK stdio server |
| `scan` | `[DIR] --format json/markdown/sarif` | Clang analysis and evidence |
| `runs` | `[DIR] [--run ID]` | List records or inspect verified record |
| `report` | `[DIR] [--run ID] --format json/markdown/sarif` | Verify and export evidence |

`scan --profile misra-c-2012` and `misra-cpp-2023` record selected editions,
produce incomplete GEP/GCS drafts and explicitly remain incomplete. With the
native module configured, `scan` runs the `misra-c2012-*` checks; their MISRA
C:2012 mappings are partial and not independently reviewed. `report` without `--run` consolidates persisted runs in
Markdown or JSON, retaining integrity errors and separate scopes. SARIF requires
one `--run`. See the formal reference for supported pinned engines and proof scope.

Exit 0 means the selected operation completed without findings or policy failure.
Exit 1 means findings remain. Exit 2 means usage/configuration/input errors,
including evidence-integrity failures. Exit 3 means execution is incomplete.
Reports retain the original run's outcome. JSON includes status and completeness.

`doctor` inventories units; its `analyzed` counter means successfully inspected
compilation entries, **not source analysis**. `doctor` records optional missing
tools visibly; a missing enabled tool makes it incomplete. Discovery of a formal
analyzer does not mean its adapter is implemented.

Configuration accepts version 1, a nonempty project name, optional assurance
standard/level, optional edition identifiers, compilation database location,
tools and execution limits, and evidence directory. Unknown fields and invalid
standard/level combinations fail validation. Edition identifiers select draft metadata and guideline mappings; they do not
establish complete enforcement and leave MISRA verification incomplete. No assurance profile is assigned by default.

Defaults: Clang enabled; formal tools disabled; timeout 60 seconds; address-space
limit 2048 MiB; output limit 4 MiB per stream; file-size limit 64 MiB; file
descriptor limit 128; networking denied. The default tool vocabulary resolves
only `clang-tidy-19`, `clang-19`, `llvm-cov-19`, `llvm-profdata-19`, `frama-c`,
`esbmc`, `safety-source-index` and `git`. A configured `tools.allowlist` replaces it with reviewed
absolute executable paths. Tool resolution follows PATH; use an explicit
allowlist and a trusted PATH on untrusted repositories.

Compilation database and evidence paths must be relative to the canonical
workspace, without `..`; evidence path components cannot be symlinks. Input
symlinks are conservatively rejected rather than silently omitted from hashing.
Source files and compilation working directories must stay inside the workspace.
System include directories may be read, but are not yet included in fingerprints.
