# SPDX-License-Identifier: Apache-2.0
"""Explicit setup: download the external SARIF schema and verify its pinned hash."""
import hashlib
import json
from pathlib import Path
import sys
import urllib.request

root = Path(__file__).resolve().parent.parent
versions = json.loads((root/"docs/tool-versions.json").read_text())
url, expected = versions["sarif_schema_url"], versions["sarif_schema_sha256"]
target = root/".cache/sarif-2.1.0.json"
if target.is_file() and hashlib.sha256(target.read_bytes()).hexdigest() == expected:
    print(f"{target} already cached and verified")
    sys.exit(0)
with urllib.request.urlopen(url, timeout=60) as response:
    data = response.read()
actual = hashlib.sha256(data).hexdigest()
if actual != expected:
    sys.exit(f"SARIF schema from {url} has SHA-256 {actual}, expected {expected}; not cached")
target.parent.mkdir(exist_ok=True)
partial = target.with_suffix(".partial")
partial.write_bytes(data)
partial.replace(target)
print(f"Cached and verified {target}")
