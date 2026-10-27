# SPDX-License-Identifier: Apache-2.0
"""Check that every stated tool version agrees with docs/tool-versions.json."""
import json
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parent.parent
versions = json.loads((root/"docs/tool-versions.json").read_text())
minimum = versions["minimum"]
problems = []

def read(name):
    return (root/name).read_text()

def expect(name, pattern, wanted, what):
    found = re.findall(pattern, read(name))
    if not found:
        problems.append(f"{name}: no {what} found (pattern {pattern!r})")
    for value in found:
        if value != wanted:
            problems.append(f"{name}: {what} is {value}, docs/tool-versions.json says {wanted}")

def rust_array(name, constant):
    match = re.search(rf"pub const {constant}: \[u32; \d+\] = \[([\d, ]+)\];", read("src/discovery.rs"))
    return ".".join(part.strip() for part in match.group(1).split(",")) if match else None

def rust_str(constant):
    match = re.search(rf'pub const {constant}: &str = "([^"]+)";', read("src/discovery.rs"))
    return match.group(1) if match else None

# Build configuration and CI.
expect("Cargo.toml", r'rust-version = "([^"]+)"', minimum["rust"], "rust-version")
expect("clang/CMakeLists.txt", r"find_package\(LLVM ([\d.]+)", minimum["llvm"], "LLVM find_package version")
expect("clang/CMakeLists.txt", r"cmake_minimum_required\(VERSION ([\d.]+)\)", minimum["cmake"], "CMake minimum")
expect(".github/workflows/ci.yml", r"rustup toolchain install ([\d.]+)", minimum["rust"], "minimum Rust toolchain")
for name in [".github/workflows/ci.yml", "packaging/reference.Dockerfile"]:
    expect(name, r"\bge ([\d.]+)", minimum["llvm"], "LLVM version gate")
expect("packaging/reference.Dockerfile", r"echo '([0-9a-f]{64})  sarif\.json'", versions["sarif_schema_sha256"], "SARIF schema hash")

# Runtime gates and validated references in the engine.
for constant, key in [("LLVM_MINIMUM", "llvm"), ("ESBMC_MINIMUM", "esbmc"), ("FRAMA_C_MINIMUM", "frama_c")]:
    value = rust_array("src/discovery.rs", constant)
    if value != minimum[key]:
        problems.append(f"src/discovery.rs: {constant} is {value}, docs/tool-versions.json says {minimum[key]}")
for constant, wanted in [("LLVM_REFERENCE", versions["llvm"]), ("ESBMC_REFERENCE", versions["esbmc"]),
                         ("FRAMA_C_REFERENCE", versions["frama_c"].split(";")[0].strip())]:
    if rust_str(constant) != wanted:
        problems.append(f"src/discovery.rs: {constant} is {rust_str(constant)}, docs/tool-versions.json says {wanted}")

# Every "X or later", "X or a later", "X+" and ">=X" statement must name a minimum
# (or its major.minor prefix, as in "19.1+").
stated = re.compile(r"(?<![\w.])(\d+\.\d+(?:\.\d+)?)(?: or (?:a )?later|\+(?![\w])| \(or later\))|>= ?(\d+\.\d+(?:\.\d+)?)")
allowed = set(minimum.values())
def is_minimum(value):
    return value in allowed or any(m.startswith(value + ".") for m in allowed)
files = [root/"README.md", root/"CONTRIBUTING.md", *sorted((root/"docs").glob("*.md")),
         *sorted((root/"skills").rglob("*.md")), *sorted((root/"src").rglob("*.rs")),
         root/"tests/integration/main.rs"]
for path in files:
    for number, line in enumerate(path.read_text().splitlines(), 1):
        for match in stated.finditer(line):
            value = match.group(1) or match.group(2)
            if not is_minimum(value):
                problems.append(f"{path.relative_to(root)}:{number}: states {value} as a minimum; "
                                f"docs/tool-versions.json minimums are {sorted(allowed)}")

if problems:
    sys.exit("Version drift:\n  " + "\n  ".join(problems))
print("Version statements agree with docs/tool-versions.json")
