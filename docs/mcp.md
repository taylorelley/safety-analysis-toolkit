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
| `safety.prove` | `source`, optional `variant`, `unwind` (8), `bits` (64) |
| `safety.coverage` | `profile`, `binary`, optional `provenance` |
| `safety.get_run` | `run_id` |
| `safety.get_findings` | `run_id`, `offset` (0), `limit` (50, maximum 100) |
| `safety.report` | `run_id`, `format` (`json`, `markdown`, `sarif`) |

Unknown input keys are rejected. Paths cannot escape the configured workspace.
Run IDs cannot contain traversal. Proof supports ESBMC 8.6.0 only over MCP.
Large results produce a structured `result_too_large` error; request smaller
pages or read integrity-checked evidence locally. The structured payload cap is
64 KiB (SDK text compatibility content may duplicate the JSON payload).

Each server serializes analyses, bounds queue wait to 20 seconds and cancels work
after 300 seconds. Request cancellation propagates to the shared runner, which
kills its process group; source hashing also checks cancellation. Per-process
limits still apply. No arbitrary executables, shell strings, test execution,
source modification or review approval are exposed.

These are configuration examples; actual harness execution has not been
validated in this environment. The real independent stdio-client integration
test covers initialization, tools/list, doctor, scan, retrieval, reporting and
strict errors. Formal/trace/coverage CLI tests use real tools separately.

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

Use a Pi MCP adapter/extension that supports stdio and configure the same command
and argv. This repository does not assume native MCP support in every Pi release
or invent a Pi-specific configuration file. Where no MCP adapter is installed,
the reusable skill can invoke the native CLI through the harness's normal tools.
Two-harness end-to-end acceptance remains outstanding.
