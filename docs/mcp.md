# MCP stdio interface

The maintained Rust MCP SDK `rmcp` 3.7.0 handles protocol negotiation and stdio.
There is no network listener. Start with a reviewed absolute workspace:

```sh
safety mcp --path /absolute/project
```

| Tool | Arguments |
| --- | --- |
| `safety.doctor` | `{}` |
| `safety.scan` | `{}` |
| `safety.trace` | `{}` |
| `safety.prove` | `source`, optional `engine` (esbmc), `method`, `entry` (main), `harness`, `variant`, `unwind` (8), `bits` (64), `precision` (Frama-C Eva, 0-11) |
| `safety.coverage` | `profile`, `binary`, optional `provenance` |
| `safety.get_run` | `run_id` |
| `safety.get_findings` | `run_id`, `offset` (0), `limit` (50, maximum 100), optional `rule_prefix`, `exclude_rule_prefixes`, `category`, `file`, `full` (false) |
| `safety.report` | `run_id`, `format` (`summary`, `json`, `markdown`, `sarif`) |

### Working with large runs

Real projects produce thousands of findings, mostly MISRA. Operation and
`get_run` summaries include `finding_counts` (counts by rule and category,
largest first), so an agent can choose filters before reading findings:

1. Read `finding_counts` from the `safety.scan` result.
2. Call `safety.get_findings` with, for example, `"rule_prefix": "clang-analyzer-"`
   or `"exclude_rule_prefixes": ["misra-"]`, optionally with `file`. `offset`,
   `total` and `next_offset` refer to the filtered set; `run_total` is unfiltered.
3. Pages are compact triage records (id, rule, category, severity, location,
   message, variant, outcome and `native_reference_count`). Pass `"full": true`
   for complete records including every native reference.
4. `safety.report` with `format: "summary"` returns a bounded Markdown overview;
   `json` returns the run summary. `markdown` and `sarif` list every finding and
   suit small runs only.

Findings located in an included file are reported once per compile
configuration: units whose commands differ only in their own source file and
output share a configuration, and a merged finding lists every reporting unit's
log in `native_references`. The run records how many findings were merged.

Unknown input keys are rejected. Paths cannot escape the configured workspace.
Run IDs cannot contain traversal. Proof supports ESBMC BMC/induction and
Frama-C Eva/WP with the same validated scope as the CLI.
Large results produce a structured `result_too_large` error that names the
bounded alternatives above; integrity-checked evidence also remains on disk. The structured payload cap is
64 KiB (SDK text compatibility content may duplicate the JSON payload).

Each server serializes analyses, bounds queue wait to 20 seconds and cancels a
tool call after `tools.mcp_operation_seconds` (default 300, 10 to 86400), read
from `safety.yaml` when the server starts. Raise it for large workspaces or
slow hosts; for reference, a 79-unit SQLite core scan took about 150 seconds on
four cores with `jobs: 4`. Request cancellation propagates to the shared runner, which
kills its process group; source hashing also checks cancellation. Per-process
limits still apply. No arbitrary executables, shell strings, test execution,
source modification or review approval are exposed.

The real independent stdio-client test covers initialization, tool discovery,
strict inputs, cancellation and bounded retrieval. `scripts/validate_harnesses.py`
exercises all eight tools through actual Codex app-server and Pi RPC hosts,
including native counterexample retrieval. It does not invoke a cloud model or
claim independent review. Records are in `validation/completion/harnesses`.

## Container

The runtime image (see [installation](installation.md#container)) starts this
server on launch. Use `docker` as the server command:

```json
{"mcpServers":{"safety":{"command":"docker","args":["run","-i","--rm","--user","1000:1000","-v","safety-esbmc:/opt/esbmc","-v","/abs/project:/abs/project","-w","/abs/project","safety-toolkit:local"]}}}
```

## Claude Code

```sh
claude mcp add --transport stdio safety -- /absolute/safety mcp --path /absolute/project
```

Equivalent MCP JSON:

```json
{"mcpServers":{"safety":{"command":"/absolute/safety","args":["mcp","--path","/absolute/project"]}}}
```

## Codex

```toml
[mcp_servers.safety]
command = "/absolute/safety"
args = ["mcp", "--path", "/absolute/project"]
```

## Hermes Agent

Use the harness's supported MCP stdio configuration; current YAML-style clients
commonly accept this server entry. Check the installed Hermes version's schema:

```yaml
mcp_servers:
  safety:
    command: /absolute/safety
    args: [mcp, --path, /absolute/project]
```

## Pi

A tested thin extension is included in `examples/agent-harnesses/pi-safety.mjs`.
It connects using the standard MCP client and registers the same tools with Pi;
there are no Pi-specific engine branches. Setup is explicit:

```sh
cd examples/agent-harnesses && npm ci
export SAFETY_BINARY=/absolute/safety SAFETY_WORKSPACE=/absolute/project
./node_modules/.bin/pi -e ./pi-safety.mjs
```

`/safety-validate {"tool":"safety.doctor","arguments":{}}` runs a deterministic
tool-host check without model inference. Normal registered tools are also
available to the host. Pi 0.73.1 and Codex 0.163.0 were exercised; Claude Code and
Hermes examples are configuration guidance and have not been executed here.
