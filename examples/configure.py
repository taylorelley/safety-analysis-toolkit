# SPDX-License-Identifier: Apache-2.0
"""Explicitly generate example compilation databases without building or testing."""
import json
from pathlib import Path
for name, extension, compiler, dialect in [
    ("embedded-c", "c", "clang-19", "c11"),
    ("embedded-cpp", "cpp", "clang++-19", "c++17"),
]:
    root = Path(__file__).resolve().parent / name
    entries = [{"directory": str(root), "file": f"src/{source}.{extension}",
        "arguments": [compiler, f"-std={dialect}", f"-DVARIANT={variant}",
        "-c", f"src/{source}.{extension}"]}
        for source in ["unsafe", "safe"] for variant in [1, 2]]
    (root / "build").mkdir(exist_ok=True)
    (root / "build/compile_commands.json").write_text(json.dumps(entries, indent=2)+"\n")
