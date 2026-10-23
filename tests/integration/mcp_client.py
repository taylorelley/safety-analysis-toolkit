# SPDX-License-Identifier: Apache-2.0
"""Real MCP stdio client: validates wire output, strict inputs and shared engine."""
import json
import os
import select
import subprocess
import sys
import time

binary, root = sys.argv[1:]
process = subprocess.Popen([binary, "mcp", "--path", root], stdin=subprocess.PIPE,
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=0)
pending = b""
def send(value):
    process.stdin.write(json.dumps({"jsonrpc": "2.0", **value}).encode() + b"\n")
    process.stdin.flush()
def receive(request_id, timeout=30):
    global pending
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if b"\n" not in pending:
            ready, _, _ = select.select([process.stdout], [], [], max(0, deadline-time.monotonic()))
            assert ready, "MCP response timeout"
            block = os.read(process.stdout.fileno(), 65536)
            assert block, "MCP server unexpectedly closed stdout"
            pending += block
            continue
        line, pending = pending.split(b"\n", 1)
        value = json.loads(line)  # No unsolicited banners or logging permitted.
        assert value["jsonrpc"] == "2.0"
        if value.get("id") == request_id:
            return value
    raise AssertionError("MCP response timeout")
def call(request_id, name, arguments):
    send({"id":request_id, "method":"tools/call", "params":{"name":name,"arguments":arguments}})
    return receive(request_id)
try:
    send({"id":1,"method":"initialize","params":{"protocolVersion":"2025-03-26",
          "capabilities":{},"clientInfo":{"name":"safety-independent-stdio-test","version":"1"}}})
    init = receive(1)
    assert init["result"]["capabilities"]["tools"] is not None
    send({"method":"notifications/initialized"})
    send({"id":2,"method":"tools/list","params":{}})
    tools = receive(2)["result"]["tools"]
    assert {tool["name"] for tool in tools} == {"safety.doctor","safety.scan","safety.trace","safety.prove","safety.coverage","safety.get_findings","safety.get_run","safety.report"}
    assert all(tool["inputSchema"].get("additionalProperties") is False for tool in tools)
    doctor = call(3,"safety.doctor",{})["result"]["structuredContent"]
    assert "complete" in doctor and doctor["completeness"]["total"] == 4
    run = call(4,"safety.scan",{})["result"]["structuredContent"]
    assert run["status"] == "findings" and run["complete"] and run["finding_count"] == 2
    run_id = run["run_id"]
    page = call(5,"safety.get_findings",{"run_id":run_id,"limit":1})["result"]["structuredContent"]
    assert len(page["findings"]) == 1 and page["next_offset"] == 1
    assert page["findings"][0]["rule_id"] == "clang-analyzer-core.NullDereference"
    lookup = call(6,"safety.get_run",{"run_id":run_id})["result"]["structuredContent"]
    assert lookup["complete"]
    report = call(7,"safety.report",{"run_id":run_id,"format":"markdown"})["result"]["structuredContent"]
    assert "Safety analysis evidence" in report["content"]
    for request_id, name, args in [(8,"safety.scan",{"command":"echo evil"}),
        (9,"safety.get_findings",{"run_id":run_id,"limit":101}),
        (10,"safety.shell",{"command":"echo evil"})]:
        result = call(request_id,name,args)
        assert "error" in result or result["result"].get("isError"), result
    traversal = call(11,"safety.get_run",{"run_id":"../../etc/passwd"})
    assert traversal["result"]["isError"]
    # Cancel a genuinely long real-Clang scan, then ensure the analysis slot frees.
    from pathlib import Path
    db_path=Path(root)/"compile_commands.json"
    db=json.loads(db_path.read_text())
    long_db=[]
    for i in range(1000):
        entry=dict(db[0])
        entry["arguments"]=[*entry["arguments"],f"-DMCP_CANCEL_CASE={i}"]
        long_db.append(entry)
    db_path.write_text(json.dumps(long_db))
    send({"id":20,"method":"tools/call","params":{"name":"safety.scan","arguments":{}}})
    time.sleep(0.2)
    send({"method":"notifications/cancelled","params":{"requestId":20,"reason":"real cancellation regression test"}})
    send({"id":21,"method":"tools/call","params":{"name":"safety.get_run","arguments":{"run_id":run_id}}})
    assert receive(21,timeout=5)["result"]["structuredContent"]["run_id"] == run_id
    print("Real MCP stdio: initialize/list/doctor/scan/findings/run/report/strict-errors passed")
finally:
    process.stdin.close()
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait()
