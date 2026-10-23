# SPDX-License-Identifier: Apache-2.0
import json
import subprocess
import sys
from pathlib import Path
exe, fixtures = sys.argv[1:]
for name, dialect in [("positive.c", "c11"), ("templates.cpp", "c++17"), ("index-complex.cpp", "c++17")]:
    result = subprocess.run([exe, str(Path(fixtures)/name), "--", f"-std={dialect}"],
        capture_output=True, text=True, timeout=15)
    assert result.returncode == 0, result.stderr
    records = [json.loads(line) for line in result.stdout.splitlines()]
    assert records and all(r["usr"] and r["line"] > 0 for r in records)
    if name=="templates.cpp":
        overloads = [r for r in records if r["name"] == "control::transform"]
        assert len({r["usr"] for r in overloads}) >= 2, records
print("Real Clang AST indexing: C functions, namespaces, templates and overload identities passed")

r=subprocess.run([exe,str(Path(fixtures)/"index-complex.cpp"),"--","-std=c++17"],capture_output=True,text=True,check=True)
records=[json.loads(line) for line in r.stdout.splitlines()]
use=next(x for x in records if x['name']=='flight::use')
assert use['unresolved_calls']>=2 and len(use['calls'])>=3,records
assert any(x['template_parent'] for x in records),records
assert any(x['macro_expansion'] for x in records),records
assert len({x['usr'] for x in records if x['name']=='flight::overload'})==2
