# SPDX-License-Identifier: Apache-2.0
"""Execute CLI and independent MCP workflows; preserve real evidence and gaps."""
import hashlib
import json
import os
from pathlib import Path
import select
import shutil
import subprocess
import time

repo=Path(__file__).resolve().parent.parent
binary=repo/"target/release/safety"
root=repo/"build/demo"
output=repo/"validation/e2e"
output.mkdir(parents=True,exist_ok=True)
commands=[("doctor",["doctor",str(root)],3),
 ("scan",["scan",str(root)],3),
 ("frama-c",["prove","src/navigation.c","--path",str(root),"--engine","frama-c"],3),
 ("esbmc-c",["prove","harness/navigation_harness.c","--path",str(root),"--engine","esbmc"],0),
 ("esbmc-cpp",["prove","src/controller.cpp","--path",str(root),"--engine","esbmc"],1),
 ("trace",["trace",str(root)],1),
 ("coverage",["coverage","import","--path",str(root),"--profile","build/test.profdata",
              "--binary","build/test","--provenance","build/provenance.json"],3)]
summary=[]
for name,args,expected in commands:
    completed=subprocess.run([binary,*args],capture_output=True,timeout=120)
    (output/f"cli-{name}.json").write_bytes(completed.stdout)
    assert completed.returncode==expected,(name,completed.returncode,completed.stdout.decode())
    data=json.loads(completed.stdout)
    summary.append({"operation":name,"interface":"CLI","exit_code":completed.returncode,
        "status":data["status"],"complete":data["complete"],"run_id":data["id"],
        "findings":len(data["findings"]),"properties":data["properties"]})
    if name=="scan":
        rules={f["rule_id"] for f in data["findings"]}
        assert "safety-prohibited-goto" in rules and "clang-analyzer-core.NullDereference" in rules
    if name=="trace":
        assert any("Broken reference" in f["message"] for f in data["findings"])
    if name=="coverage":
        assert any(f["rule_id"]=="coverage-gap-mcdc" for f in data["findings"])
process=subprocess.Popen([binary,"mcp","--path",root],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,bufsize=0)
pending=b""
def send(message):
    process.stdin.write(json.dumps({"jsonrpc":"2.0",**message}).encode()+b"\n")
    process.stdin.flush()
def receive(request_id):
    global pending
    deadline=time.monotonic()+120
    while time.monotonic()<deadline:
        if b"\n" not in pending:
            ready,_,_=select.select([process.stdout],[],[],max(0,deadline-time.monotonic()))
            assert ready,"MCP timeout"
            block=os.read(process.stdout.fileno(),65536)
            assert block,"MCP closed"
            pending+=block
            continue
        line,pending=pending.split(b"\n",1)
        value=json.loads(line)
        if value.get("id")==request_id:return value
    raise AssertionError("MCP timeout")
try:
    send({"id":1,"method":"initialize","params":{"protocolVersion":"2025-03-26","capabilities":{},"clientInfo":{"name":"safety-e2e-independent-client","version":"1"}}})
    receive(1)
    send({"method":"notifications/initialized"})
    calls=[("safety.doctor",{}),("safety.scan",{}),
        ("safety.prove",{"source":"harness/navigation_harness.c"}),
        ("safety.prove",{"source":"src/controller.cpp"}),
        ("safety.trace",{}),("safety.coverage",{"profile":"build/test.profdata",
           "binary":"build/test","provenance":"build/provenance.json"})]
    for request_id,(name,args) in enumerate(calls,2):
        send({"id":request_id,"method":"tools/call","params":{"name":name,"arguments":args}})
        result=receive(request_id)
        (output/f"mcp-{request_id}.json").write_text(json.dumps(result,indent=2)+"\n")
        assert "error" not in result and not result["result"].get("isError"),result
        data=result["result"]["structuredContent"]
        summary.append({"operation":name,"interface":"MCP","status":data["status"],
            "complete":data["complete"],"run_id":data["run_id"]})
finally:
    process.stdin.close()
    try:process.wait(timeout=5)
    except subprocess.TimeoutExpired:process.kill();process.wait()
report=subprocess.run([binary,"report",root,"--format","markdown"],capture_output=True,timeout=30)
assert report.returncode==3
(output/"consolidated-report.md").write_bytes(report.stdout)
(output/"summary.json").write_text(json.dumps({"status":"incomplete","complete":False,
    "reason":"Frama-C unvalidated, MISRA mappings unvalidated, known trace and MC/DC gaps; two real agent harnesses not exercised",
    "results":summary},indent=2)+"\n")
# Copy only generated records, never analyzer binaries or a third-party bundle.
shutil.copytree(root/".safety/runs",output/"runs",dirs_exist_ok=True)
for directory in (output/"runs").iterdir():
    inventory=json.loads((directory/"evidence.json").read_text())
    for name,expected in inventory.items():
        assert hashlib.sha256((directory/name).read_bytes()).hexdigest()==expected
print("CLI and independent MCP workflows executed; native evidence verified; overall incomplete scope preserved")
