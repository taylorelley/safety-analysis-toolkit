# SPDX-License-Identifier: Apache-2.0
"""Smoke-test the runtime image as an MCP client would use it.

Starts the container twice over stdio with a fresh workspace and a named ESBMC
volume: the first start fetches ESBMC and creates safety.yaml, the second must
reuse both. stdout must carry only JSON-RPC, doctor must be complete (every
capability present) and scan must analyse the workspace.

    python3 scripts/container_smoke.py IMAGE
"""
import json
import os
import subprocess
import sys
import tempfile
import time
from pathlib import Path

image = sys.argv[1]
volume = f"safety-smoke-esbmc-{os.getpid()}"
requests = [
    {"jsonrpc": "2.0", "id": 1, "method": "initialize", "params": {
        "protocolVersion": "2025-03-26", "capabilities": {}, "clientInfo": {"name": "smoke", "version": "1"}}},
    {"jsonrpc": "2.0", "method": "notifications/initialized"},
    {"jsonrpc": "2.0", "id": 2, "method": "tools/list"},
    {"jsonrpc": "2.0", "id": 3, "method": "tools/call", "params": {"name": "safety.doctor", "arguments": {}}},
    {"jsonrpc": "2.0", "id": 4, "method": "tools/call", "params": {"name": "safety.scan", "arguments": {}}},
]


def start(workspace: Path) -> tuple[dict, str]:
    command = ["docker", "run", "-i", "--rm", "--user", f"{os.getuid()}:{os.getgid()}",
               "-v", f"{volume}:/opt/esbmc", "-v", f"{workspace}:{workspace}", "-w", str(workspace), image]
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, text=True)
    responses = {}
    try:
        for request in requests:
            process.stdin.write(json.dumps(request) + "\n")
            process.stdin.flush()
            if "id" not in request:
                continue
            deadline = time.monotonic() + 600
            while request["id"] not in responses:
                assert time.monotonic() < deadline, f"timeout waiting for response {request['id']}"
                line = process.stdout.readline()
                assert line, "server closed stdout"
                message = json.loads(line)  # anything other than JSON-RPC on stdout fails here
                assert message.get("jsonrpc") == "2.0", line
                responses[message.get("id")] = message
    finally:
        process.stdin.close()
        stderr = process.stderr.read()
        process.wait(timeout=60)
    return responses, stderr


try:
    with tempfile.TemporaryDirectory() as directory:
        workspace = Path(directory)
        (workspace / "build").mkdir()
        (workspace / "value.c").write_text("int value(int *p) { return *p; }\n")
        (workspace / "build/compile_commands.json").write_text(json.dumps([{
            "directory": str(workspace), "file": "value.c",
            "arguments": ["clang-19", "-std=c11", "-c", "value.c"]}]))
        for attempt in ("first", "second"):
            responses, stderr = start(workspace)
            tools = {tool["name"] for tool in responses[2]["result"]["tools"]}
            assert len(tools) == 8, tools
            doctor = responses[3]["result"]["structuredContent"]
            assert doctor["complete"], (attempt, doctor["limitations"], stderr)
            scan = responses[4]["result"]["structuredContent"]
            assert scan["completeness"]["analyzed"] == 1, (attempt, scan, stderr)
            if attempt == "first":
                assert "fetch_esbmc: installed" in stderr, stderr
                config = (workspace / "safety.yaml").read_text()
                assert "frama_c: true" in config and "esbmc: true" in config, config
            else:
                assert "fetch_esbmc" not in stderr, stderr
            print(f"{attempt} start: 8 tools, doctor complete, scan {scan['status']} "
                  f"({scan['finding_count']} findings), clean stdout")
finally:
    subprocess.run(["docker", "volume", "rm", "-f", volume], capture_output=True)
print("Container MCP smoke test passed")
