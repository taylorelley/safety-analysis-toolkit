# SPDX-License-Identifier: Apache-2.0
"""Real instrumentation and execution, explicitly part of development tests."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import os

binary = sys.argv[1]
with tempfile.TemporaryDirectory() as directory:
    root = Path(directory)
    source = root / "decision.c"
    source.write_text("int decide(int a,int b){if(a&&b)return 1;return 0;}\nint main(void){return decide(0,0)+decide(1,1)==1?0:1;}\n")
    args = ["clang-19", "-std=c11", "-fprofile-instr-generate", "-fcoverage-mapping",
            "-fcoverage-mcdc", str(source), "-o", str(root/"test")]
    subprocess.run(args, check=True, timeout=30)
    # User-authorized development fixture execution; safety itself never does this.
    subprocess.run([root/"test"], env={**os.environ,
        "LLVM_PROFILE_FILE":str(root/"test.profraw")}, check=True, timeout=5)
    subprocess.run(["llvm-profdata-19","merge","-sparse",root/"test.profraw",
        "-o",root/"test.profdata"], check=True, timeout=10)
    db = [{"directory":directory,"file":str(source),"arguments":args}]
    (root/"compile_commands.json").write_text(json.dumps(db))
    (root/"safety.yaml").write_text("version: 1\nproject: {name: mcdc-known-gap}\nbuild: {compile_commands: compile_commands.json}\n")
    def sha(path):
        return hashlib.sha256((root/path).read_bytes()).hexdigest()
    provenance = {"version":1,"binary_sha256":sha("test"),
        "profile_sha256":sha("test.profdata"),"sources":{"decision.c":sha("decision.c")},
        "compile_commands_sha256":sha("compile_commands.json"),
        "execution_environment":"host","build_variant":"host-mcdc-test",
        "instrumentation_warnings":[],"exclusions":[],"test_ids":["TST-DEC-1"],"requirement_ids":["REQ-DEC-1"]}
    (root/"provenance.json").write_text(json.dumps(provenance))
    args = [binary,"coverage","import","--path",directory,"--profile","test.profdata",
            "--binary","test"]
    incomplete = subprocess.run(args, capture_output=True, timeout=30)
    assert incomplete.returncode == 3, incomplete.stdout.decode()
    result = subprocess.run(args+["--provenance","provenance.json"],capture_output=True,timeout=30)
    assert result.returncode == 1, result.stdout.decode()
    run = json.loads(result.stdout)
    assert run["complete"] and run["completeness"]["analyzed"] == 1
    assert any(f["rule_id"] == "coverage-gap-mcdc" for f in run["findings"])
    exported = json.loads((root/".safety/runs"/run["id"]/"coverage.json").read_text())
    assert exported["export"]["data"][0]["totals"]["mcdc"]["percent"] == 50
    (root/"test.profdata").write_text("corrupted profile")
    failed = subprocess.run(args,capture_output=True,timeout=30)
    assert failed.returncode == 3
    assert json.loads(failed.stdout)["complete"] is False
    print("Real LLVM source/MC/DC import: 50% MC/DC gap, missing provenance and corrupt profile validated")
