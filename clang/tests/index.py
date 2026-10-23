# SPDX-License-Identifier: Apache-2.0
import json
import subprocess
import sys
from pathlib import Path
exe, fixtures = sys.argv[1:]
for name, dialect in [("positive.c", "c11"), ("templates.cpp", "c++17")]:
    result = subprocess.run([exe, str(Path(fixtures)/name), "--", f"-std={dialect}"],
        capture_output=True, text=True, timeout=15)
    assert result.returncode == 0, result.stderr
    records = [json.loads(line) for line in result.stdout.splitlines()]
    assert records and all(r["usr"] and r["line"] > 0 for r in records)
    if name.endswith("cpp"):
        overloads = [r for r in records if r["name"] == "control::transform"]
        assert len({r["usr"] for r in overloads}) >= 2, records
print("Real Clang AST indexing: C functions, namespaces, templates and overload identities passed")
