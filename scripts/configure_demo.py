# SPDX-License-Identifier: Apache-2.0
"""Explicit developer setup: copies fixtures, builds and executes coverage tests."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

repo = Path(__file__).resolve().parent.parent
root = repo/"build/demo"
root.mkdir(parents=True, exist_ok=True)
shutil.copytree(repo/"examples/mixed-demo/src", root/"src", dirs_exist_ok=True)
shutil.copytree(repo/"examples/mixed-demo/harness", root/"harness", dirs_exist_ok=True)
shutil.copyfile(repo/"examples/mixed-demo/trace.yaml", root/"trace.yaml")
(root/"build").mkdir(exist_ok=True)
flags = ["-fprofile-instr-generate","-fcoverage-mapping","-fcoverage-mcdc"]
entries = []
for name in ["navigation.c","controller.cpp","decision.c","test.c","contract.c"]:
    cpp = name.endswith("cpp")
    entries.append({"directory":str(root),"file":f"src/{name}",
        "arguments":["clang++-19" if cpp else "clang-19",
        "-std=c++17" if cpp else "-std=c11","-DSAFETY_DEMO=1",
        *(flags if name in ["decision.c","test.c"] else []),"-c",f"src/{name}"]})
entries.append({"directory":str(root),"file":"harness/navigation_harness.c",
    "arguments":["clang-19","-std=c11","-DSAFETY_DEMO=1","-c","harness/navigation_harness.c"]})
(root/"build/compile_commands.json").write_text(json.dumps(entries,indent=2)+"\n")
module = (repo/"build/clang/SafetyMisra.so").resolve(strict=True)
allow = [str(module)]
for name in ["clang-tidy-19","clang-19","safety-source-index","llvm-cov-19",
             "llvm-profdata-19","esbmc","git","frama-c"]:
    tool = shutil.which(name)
    if tool:
        allow.append(str(Path(tool).resolve()))
config = {"version":1,"project":{"name":"mixed-safety-demo"},
    "coding":{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"},
    "tools":{"clang":True,"frama_c":True,"esbmc":True,
             "clang_module":str(module),"allowlist":allow}}
(root/"safety.yaml").write_text(json.dumps(config,indent=2)+"\n")
subprocess.run(["clang-19","-std=c11","-DSAFETY_DEMO=1",*flags,
    root/"src/decision.c",root/"src/test.c","-o",root/"build/test"],check=True,timeout=30)
subprocess.run([root/"build/test"],env={**os.environ,"LLVM_PROFILE_FILE":str(root/"build/test.profraw")},check=True,timeout=10)
subprocess.run(["llvm-profdata-19","merge","-sparse",root/"build/test.profraw",
               "-o",root/"build/test.profdata"],check=True,timeout=10)
def sha(name):
    return hashlib.sha256((root/name).read_bytes()).hexdigest()
provenance = {"version":1,"binary_sha256":sha("build/test"),
    "profile_sha256":sha("build/test.profdata"),
    "sources":{f"src/{name}":sha(f"src/{name}") for name in
        ["navigation.c","controller.cpp","decision.c","test.c","contract.c"]},
    "compile_commands_sha256":sha("build/compile_commands.json"),
    "execution_environment":"host","build_variant":"SAFETY_DEMO=1",
    "instrumentation_warnings":[],"exclusions":[],"test_ids":["T-DEC"],
    "requirement_ids":["R-DEC"]}
provenance["sources"]["harness/navigation_harness.c"]=sha("harness/navigation_harness.c")
(root/"build/provenance.json").write_text(json.dumps(provenance,indent=2)+"\n")
print(root)
