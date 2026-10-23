# Interface notes

Start `safety mcp --path /absolute/reviewed/workspace`. MCP tools:
`safety.doctor`, `safety.scan`, `safety.prove`, `safety.trace`, `safety.coverage`,
`safety.get_findings`, `safety.get_run`, `safety.report`.

Use tools/list to obtain authoritative schemas. Operation tools default to the
server's fixed workspace. Proof and coverage source/profile paths must be
relative, existing and contained. MCP proof supports pinned ESBMC BMC/induction and Frama-C Eva/WP.

CLI JSON has `status`, `complete`, `completeness`, `findings` and `properties`.
MCP operation summaries use `run_id` and evidence references. Findings pages
have `offset`, `next_offset`, `total`. Exit codes: 0 selected operation passed,
1 findings remain, 2 invalid usage/input/evidence, 3 incomplete.

The CLI supports `--format json`, Markdown and per-run SARIF for analysis and
reports. Consolidated Markdown/JSON include all persisted runs with their
separate scopes, including integrity errors and incomplete operations.

No MCP session is granted change or review approval. The tool's network-denial
filter is not a general sandbox. Filesystem isolation depends on Landlock;
strict projects set `tools.require_filesystem_isolation: true`.
