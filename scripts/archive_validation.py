# SPDX-License-Identifier: Apache-2.0
"""Archive real CLI/host records without replacing a completed run."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import jsonschema

repo = Path(__file__).resolve().parent.parent
root = repo/'build/demo'
output = repo/'validation/completion/e2e'
(output/'runs').mkdir(parents=True, exist_ok=True)
evidence_schema = json.loads((repo/'schemas/evidence.schema.json').read_text())
finding_schema = json.loads((repo/'schemas/findings.schema.json').read_text())
for original in sorted((root/'.safety/runs').iterdir()):
    destination = output/'runs'/original.name
    if destination.exists():
        assert (destination/'evidence.json').read_bytes() == (original/'evidence.json').read_bytes()
    else:
        shutil.copytree(original, destination)
    inventory = json.loads((destination/'evidence.json').read_text())
    for name, expected in inventory.items():
        assert hashlib.sha256((destination/name).read_bytes()).hexdigest() == expected
    run = json.loads((destination/'manifest.json').read_text())
    jsonschema.validate(run, evidence_schema)
    jsonschema.validate(run['findings'], finding_schema)
snapshot = output/'project-inputs'
if not snapshot.exists():
    shutil.copytree(root, snapshot, ignore=shutil.ignore_patterns('.safety'))
# Report-only archive configuration. Reports derive their scope from manifests.
(output/'safety.yaml').write_text('version: 1\nproject: {name: archived-mixed-demo}\nevidence: {directory: runs}\ntools: {clang: false}\n')
for format, filename in [('markdown','consolidated-report.md'),('json','consolidated-report.json')]:
    result = subprocess.run([repo/'target/release/safety','report',output,'--format',format],
        capture_output=True, timeout=60)
    assert result.returncode == 3, (result.returncode,result.stdout,result.stderr)
    (output/filename).write_bytes(result.stdout)
print(f'{len(list((output/"runs").iterdir()))} real runs archived, schema-validated and integrity-verified; source/profile/binary inputs retained; reports regenerated offline')
