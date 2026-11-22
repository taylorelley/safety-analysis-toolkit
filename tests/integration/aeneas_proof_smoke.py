"""Extract and prove the Rust interlock's refinement with Hax/Aeneas and Lean."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tempfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--work-dir", type=Path, help="Dedicated extraction directory with room for Lake dependencies")
args = parser.parse_args()

repo = Path(__file__).resolve().parents[2]
safety = repo / "target/debug/safety"
example = repo / "examples/rust-interlock"
proposition = ("∀ (inhibited fault reset : Bool), interlock.next_inhibit inhibited fault reset "
               "= .ok (fault || (inhibited && !reset))")
toolchain = {
    "rustc": "nightly-2026-09-17", "cargo": "hax extraction toolchain",
    "lean": "4.35.0", "hax": "0.5.0", "charon": "nightly-2026.10.31",
    "aeneas": "build-2b9e3ea", "hax-lean-lib": "v0.3.41",
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def model_inventory(generated):
    return {p.relative_to(generated).as_posix(): digest(p.read_bytes())
            for p in sorted((generated / "Interlock/Extraction").rglob("*.lean"))}


def run(work):
    (work / "src").mkdir(parents=True, exist_ok=True)
    shutil.copy(example / "src/lib.rs", work / "src/lib.rs")
    (work / "Cargo.toml").write_text('[package]\nname = "interlock"\nversion = "0.1.0"\nedition = "2024"\n')
    (work / "hax.toml").write_text('[scenario.interlock]\nbackend = "lean"\n')
    generated = work / "proofs/interlock/lean"
    extract = ["cargo", "hax", "extract", "interlock"]
    first = subprocess.run(extract, cwd=work, capture_output=True, check=True, text=True)
    reviewed = model_inventory(generated)
    assert reviewed, "Aeneas did not generate Lean models"
    for path in reviewed:
        (generated / path).unlink()
    second = subprocess.run(extract, cwd=work, capture_output=True, check=True, text=True)
    regenerated = model_inventory(generated)
    assert reviewed == regenerated, "Model regeneration changed the reviewed extraction"
    probe = (f"import Interlock.Proof\nset_option warningAsError true\n"
             f"#check (@interlock_refines : {proposition})\n#print axioms interlock_refines\n")
    shutil.copy(example / "proofs/lean/Proof.lean", generated / "Interlock/Proof.lean")
    (generated / "Probe.lean").write_text(probe)
    subprocess.run(["lake", "build", "Interlock.Proof"], cwd=generated, check=True)
    command = ["lake", "env", "lean", "--json", "Probe.lean"]
    proof = subprocess.run(command, cwd=generated, capture_output=True, text=True, check=True)

    with tempfile.TemporaryDirectory() as scratch:
        root = Path(scratch)
        shutil.copy(example / "safety.yaml", root)
        shutil.copy(work / "src/lib.rs", root / "lib.rs")
        for config in ("Cargo.toml", "Cargo.lock", "hax.toml"):
            shutil.copy(work / config, root)
        for config in ("lakefile.toml", "lake-manifest.json", "lean-toolchain"):
            shutil.copy(generated / config, root)
        sources = [*reviewed, "Interlock/Assumptions/FunsExternal.lean", "Interlock/Proof.lean",
                   "Probe.lean", "llbc/interlock.llbc"]
        for path in sources:
            (root / path).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(generated / path, root / path)
        plan = {"version": 1, "invocations": {"refines": {
            "variant":"host", "command":command, "toolchain":toolchain,
            "checks":{"format":"aeneas_lean", "source":"lib.rs", "models":reviewed,
                      "proof":"Interlock/Proof.lean", "probe":"Probe.lean",
                      "module":"Interlock.Proof", "theorem":"interlock_refines",
                      "requirement":"REQ-KEEP", "proposition":proposition,
                      "allowed_axioms":["propext", "Quot.sound"]},
        }}}
        (root / "rust-verification.json").write_text(json.dumps(plan))
        inputs = json.loads(subprocess.check_output([safety, "rust", "snapshot", "--path", root]))
        stale = regenerated.copy()
        stale[next(iter(stale))] = "0" * 64
        for model, outcome in [(regenerated, "PROVEN"), (stale, "FAILED")]:
            bundle = {"version":1, "producer":"local Hax/Aeneas/Lean replay", "inputs":inputs,
                      "invocations":{"refines":{
                          "command":command, "toolchain":toolchain, "exit_code":proof.returncode,
                          "output":proof.stdout, "output_sha256":digest(proof.stdout.encode()),
                          "regenerated_models":model,
                          "diagnostics":first.stderr + second.stderr + proof.stderr,
                      }}}
            (root / "bundle.json").write_text(json.dumps(bundle))
            result = subprocess.run([safety, "rust", "import", "bundle.json", "--path", root],
                                    capture_output=True, text=True)
            run_record = json.loads(result.stdout)
            assert result.returncode == (0 if outcome == "PROVEN" else 3), run_record
            assert run_record["properties"][0]["outcome"] == outcome, run_record
            print("Aeneas/Lean refinement:", outcome)


if args.work_dir:
    run(args.work_dir.resolve())
else:
    with tempfile.TemporaryDirectory(dir=Path.cwd()) as temp:
        run(Path(temp))
