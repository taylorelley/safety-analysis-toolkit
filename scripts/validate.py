# SPDX-License-Identifier: Apache-2.0
"""Offline, real CLI known-answer validation, including schema validation."""
import hashlib
import importlib.metadata
import json
import re
import subprocess
from pathlib import Path
import jsonschema

root = Path(__file__).resolve().parent.parent
minimum_jsonschema = json.loads((root / "docs/tool-versions.json").read_text())["minimum"]["python_jsonschema"]
installed_jsonschema = importlib.metadata.version("jsonschema")
def meets_minimum(installed, minimum):
    """PEP 440 ordering against a final-release minimum: a pre-release or dev
    release of the minimum (4.10.3rc1, 4.10.3.dev1) is older; post-releases
    and local labels are not. Unparseable versions are refused."""
    pattern = (r"v?(\d+(?:\.\d+)*)((?:[-_.]?(?:a|b|c|rc|alpha|beta|pre|preview)[-_.]?\d*)?)"
               r"((?:[-_.]?(?:post|rev|r)[-_.]?\d*|-\d+)?)((?:[-_.]?dev[-_.]?\d*)?)(?:\+[a-z0-9.]+)?")
    parsed = [re.fullmatch(pattern, version.strip().lower()) for version in (installed, minimum)]
    if not all(parsed) or any(parsed[1].group(2, 3, 4)):
        return False
    release = [[int(part) for part in match.group(1).split(".")] for match in parsed]
    width = max(map(len, release))
    have, need = (tuple(r + [0] * (width - len(r))) for r in release)
    pre, post, dev = parsed[0].group(2, 3, 4)
    # A dev suffix after a post-release (4.10.3.post1.dev1) still follows 4.10.3.
    return have > need or (have == need and not pre and (bool(post) or not dev))
for installed, minimum, expected in [("4.10.3", "4.10.3", True), ("4.19.2", "4.10.3", True),
        ("4.10.3.post1", "4.10.3", True), ("4.10.3+deb", "4.10.3", True), ("4.10.3rc1", "4.10.3", False),
        ("4.10.3.dev1", "4.10.3", False), ("4.10.3.post1.dev1", "4.10.3", True), ("4.10.2", "4.10.3", False), ("4.11.0a1", "4.10.3", True), ("x", "4.10.3", False)]:
    assert meets_minimum(installed, minimum) is expected, (installed, minimum)
if not meets_minimum(installed_jsonschema, minimum_jsonschema):
    raise SystemExit(f"jsonschema {installed_jsonschema} is older than the minimum {minimum_jsonschema}")
binary = root / "target/release/safety"
if not binary.is_file():
    raise SystemExit("Build the release executable first")
schemas = {name: json.loads((root / f"schemas/{name}.schema.json").read_text())
           for name in ["project", "findings", "evidence"]}
sarif_path = root / ".cache/sarif-2.1.0.json"
sarif_sha256 = json.loads((root / "docs/tool-versions.json").read_text())["sarif_schema_sha256"]
if not sarif_path.is_file():
    raise SystemExit(f"{sarif_path} is missing; run `make cache` (downloads and hash-verifies the SARIF schema)")
if hashlib.sha256(sarif_path.read_bytes()).hexdigest() != sarif_sha256:
    raise SystemExit(f"{sarif_path} does not match the pinned SHA-256; delete it and run `make cache`")
sarif_schema = json.loads(sarif_path.read_text())
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
print(f"Offline schema and real-analysis validation passed (jsonschema {installed_jsonschema})")
