# SPDX-License-Identifier: Apache-2.0
"""Known-answer real plugin checks; nonzero compiler exits are never ignored."""
import subprocess
import sys
from pathlib import Path

exe, module, fixtures = sys.argv[1:]
for filename, count, valid in [
    ("positive.c", 1, True), ("negative.c", 0, True),
    ("boundary.c", 1, True), ("macro.c", 1, True),
    ("templates.cpp", 1, True), ("invalid.c", 0, False),
]:
    path = Path(fixtures) / filename
    dialect = "-std=c++17" if path.suffix == ".cpp" else "-std=c11"
    result = subprocess.run([exe, f"--load={module}",
        "--checks=-*,safety-prohibited-goto", "--config={}", str(path),
        "--", dialect], capture_output=True, text=True, timeout=15)
    text = result.stdout + result.stderr
    actual = text.count("[safety-prohibited-goto]")
    assert actual == count, (filename, actual, text)
    assert (result.returncode == 0) == valid, (filename, result.returncode, text)
print("6 real clang-tidy plugin fixtures passed")
