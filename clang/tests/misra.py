# SPDX-License-Identifier: Apache-2.0
"""Known-answer fixtures for the MISRA C:2012 guideline checks.

Each fixture in misra/ starts with a ``CHECKS:`` comment naming the checks it
exercises and may add an ``ARGS:`` comment with compiler arguments (default
-std=c99). Every line containing the marker NONCOMPLIANT, in the fixture or in
its companion headers (``<stem>_*.h``), must receive a diagnostic from one of
those checks and no other line may receive one. Every registered misra check
must be exercised by at least one fixture with at least one positive line.
"""
import re
import subprocess
import sys
from pathlib import Path

exe, module, base = sys.argv[1:]
fixtures = Path(base) / "misra"
diagnostic = re.compile(r"^(.*?):(\d+):\d+: (?:warning|error): .*\[([\w.,-]+)\]$")
listed = subprocess.run(
    [exe, f"--load={module}", "--checks=-*,misra-c2012-*", "--list-checks"],
    capture_output=True, text=True, timeout=60, check=True).stdout
registered = set(re.findall(r"misra-c2012-[\w-]+", listed))
covered = set()
failures = []
for source in sorted(fixtures.glob("*.c")):
    text = source.read_text()
    checks = re.search(r"CHECKS:\s*([\w.,*-]+)", text).group(1).split(",")
    args = re.search(r"ARGS:\s*(.*?)\s*(?:\*/)?$", text, re.M)
    args = args.group(1).split() if args else ["-std=c99"]
    files = [source] + sorted(fixtures.glob(source.stem + "_*.h"))
    expected = {(f.name, n) for f in files
                for n, line in enumerate(f.read_text().splitlines(), 1) if "NONCOMPLIANT" in line}
    result = subprocess.run(
        [exe, f"--load={module}", "--allow-no-checks", "--checks=-*," + ",".join(checks),
         "--config={HeaderFilterRegex: '.*'}", str(source), "--", *args, f"-I{fixtures}"],
        capture_output=True, text=True, timeout=120)
    actual = set()
    for line in (result.stdout + result.stderr).splitlines():
        match = diagnostic.match(line)
        if match and any(c in checks or c.startswith(tuple(p.rstrip("*") for p in checks if p.endswith("*")))
                         for c in match.group(3).split(",")):
            actual.add((Path(match.group(1)).name, int(match.group(2))))
    if expected != actual or not expected:
        failures.append(f"{source.name}: missing {sorted(expected - actual)} unexpected {sorted(actual - expected)}\n"
                        + result.stdout + result.stderr)
    if result.returncode != 0 and "clang-diagnostic-error" not in checks:
        failures.append(f"{source.name}: clang-tidy exit {result.returncode}\n{result.stderr}")
    covered.update(c for c in checks if c.startswith("misra-c2012-"))
missing = sorted(registered - covered)
if missing:
    failures.append(f"checks without fixtures: {missing}")
if failures:
    print("\n\n".join(failures))
    sys.exit(1)
print(f"{len(registered)} MISRA C:2012 checks passed {len(list(fixtures.glob('*.c')))} known-answer fixtures")
