# SPDX-License-Identifier: Apache-2.0
"""Offline, real CLI known-answer validation, including schema validation."""
import hashlib
import json
import subprocess
from pathlib import Path
import jsonschema

root = Path(__file__).resolve().parent.parent
binary = root / "target/release/safety"
if not binary.is_file():
    raise SystemExit("Build the release executable first")
schemas = {name: json.loads((root / f"schemas/{name}.schema.json").read_text())
           for name in ["project", "findings", "evidence"]}
sarif_path = root / ".cache/sarif-2.1.0.json"
assert hashlib.sha256(sarif_path.read_bytes()).hexdigest() == "c96eb2d311c37b0a38cbd18c52d79a68f778bbd2d831abed7412d9850740f785", "Unexpected SARIF schema version"
sarif_schema = json.loads((root / ".cache/sarif-2.1.0.json").read_text())
for schema in [*schemas.values(), sarif_schema]:
    jsonschema.validators.validator_for(schema).check_schema(schema)
for project in ["embedded-c", "embedded-cpp"]:
    path = root / "examples" / project
    result = subprocess.run([binary, "scan", path], capture_output=True, timeout=120)
    assert result.returncode == 1, result.stdout.decode()
    run = json.loads(result.stdout)
    jsonschema.validate(run, schemas["evidence"])
    jsonschema.validate(run["findings"], schemas["findings"])
    assert run["complete"] and run["completeness"]["analyzed"] == 4
    assert len(run["findings"]) == 2
    assert all(f["rule_id"] == "clang-analyzer-core.NullDereference"
               for f in run["findings"])
    for format in ["json", "sarif", "markdown"]:
        report = subprocess.run([binary, "report", path, "--run", run["id"],
            "--format", format], capture_output=True, timeout=10)
        assert report.returncode == 1, report.stdout.decode()
        if format == "sarif":
            jsonschema.validate(json.loads(report.stdout), sarif_schema)
    directory = path / ".safety/runs" / run["id"]
    inventory = json.loads((directory / "evidence.json").read_text())
    for name, expected in inventory.items():
        assert hashlib.sha256((directory / name).read_bytes()).hexdigest() == expected
    print(f"{project}: 4/4 units; 2 expected findings; JSON/SARIF/report/integrity valid")
# A bad config must be rejected by both the published schema and the CLI.
try:
    jsonschema.validate({"version": 1, "project": {"name": "x"},
        "assurance": {"standard": "DO-278A", "level": "C"}}, schemas["project"])
except jsonschema.ValidationError:
    pass
else:
    raise AssertionError("Schema accepted incompatible assurance level")
print("Offline schema and real-analysis validation passed")
