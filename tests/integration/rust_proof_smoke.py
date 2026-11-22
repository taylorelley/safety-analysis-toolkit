from pathlib import Path
"""Replay the known-answer interlock against Kani 0.68.0 and CBMC 6.11.0."""

import hashlib
import json
import shutil
import argparse
import subprocess
import tempfile

parser = argparse.ArgumentParser(description="Replay the pinned interlock proofs")
parser.add_argument("--with-esbmc", action="store_true")
parser.add_argument("--with-verus", action="store_true")
parser.add_argument("--evidence-dir", type=Path)
args = parser.parse_args()

def record(command, tools, result, output):
    return {"command":command,"toolchain":tools,"exit_code":result.returncode,
            "output_sha256":hashlib.sha256(output.encode()).hexdigest(),"output":output}

safety = Path("target/debug/safety").resolve()
source = Path("examples/rust-interlock").resolve()
with tempfile.TemporaryDirectory() as tmp:
    temp = Path(tmp)
    root = temp / "workspace"
    shutil.copytree(source, root)
    work = temp / "verifier"
    work.mkdir()
    bundle_file = root / "bundle.json"
    assert subprocess.check_output(["cbmc", "--version"], text=True).startswith("6.11.0")
    assert "0.68.0" in subprocess.check_output(["kani-driver", "--version"], text=True)
    proof_command = ["kani-driver", "--keep-temps", "--only-codegen", "lib.rs"]
    tools = {"kani":"0.68.0","cbmc":"6.11.0", "rustc":subprocess.check_output(["rustc","-vV"], text=True).strip(),
             "cargo":subprocess.check_output(["cargo","-V"], text=True).strip()}
    obligations = {}
    plan_entry = {"variant":"host", "command":[], "toolchain":tools,
                  "checks":{"format":"kani_cbmc","harness":"proofs::reset_transition", "unwind":3,
                            "completeness":[], "safety_checks":[], "obligations":obligations}}
    plan = {"version":1,"invocations":{"proof":plan_entry}}
    for broken in [False, True]:
        if broken:
            lib = root / "src/lib.rs"
            lib.write_text(lib.read_text().replace("fault || (inhibited && !reset)", "(inhibited && !reset)"))
            bundle_file.unlink()
            for path in work.glob("*.out"):
                path.unlink()
        shutil.copy(root / "src/lib.rs", work / "lib.rs")
        subprocess.run(proof_command, cwd=work, check=True, capture_output=True)
        metadata = json.loads((work / "lib.kani-metadata.json").read_text())
        assert metadata["unsupported_features"] == []
        harness = metadata["proof_harnesses"][0]
        model = Path(harness["goto_file"]).name.replace(".symtab.out", ".out")
        checker = ["cbmc",model,"--function",harness["mangled_name"],"--unwind","3","--unwinding-assertions","--trace","--json-ui"]
        result = subprocess.run(checker, cwd=work, capture_output=True, text=True)
        assert result.returncode == 10, result.stderr
        checks = next(r["result"] for r in json.loads(result.stdout) if "result" in r)
        if not broken:
            plan_entry["command"] = checker
            for i, req in enumerate(["REQ-KEEP","REQ-CLEAR"], 1):
                obligations[req] = {"requirement":req,"proposition":req+" holds for reset",
                    "native_check":f"proofs::reset_transition.assertion.{i}",
                    "reachability":[f"proofs::reset_transition.cover.{i}", f"proofs::reset_transition.reachability_check.{i}"],
                    "assumptions":["Fault and reset are sampled simultaneously."]}
            plan_entry["checks"]["safety_checks"] = [p["property"] for p in checks if p["property"].startswith("__rust_")]

        else:
            assert plan_entry["command"] == checker
        records = {"proof": record(checker, tools, result, result.stdout)}
        if args.with_esbmc:
            shutil.copy(work / model, root / model)
            replay = ["esbmc", "--binary", model, "--function", harness["mangled_name"],
                      "--ignore-property-class", "reachability_check",
                      "--ignore-property-class", "cover", "--unwind", "3", "--multi-property"]
            es = subprocess.run(replay, cwd=work, capture_output=True, text=True, timeout=60)
            assert es.returncode == int(broken), es.stdout + es.stderr
            output = es.stderr + es.stdout
            es_tools = {**tools, "esbmc":"8.7.0"}
            plan["invocations"]["replay"] = {
                "variant":"host", "command":replay, "toolchain":es_tools,
                "checks":{"format":"esbmc_goto", "cbmc":"proof", "model":model,
                          "model_sha256":hashlib.sha256((root / model).read_bytes()).hexdigest(),
                          "entry":harness["mangled_name"],
                          "properties":sorted(o["native_check"] for o in obligations.values()),
                          "allowed_warnings":[line for line in output.splitlines()
                             if line.startswith(("WARNING: no definition for external symbol signgam declared at file ",
                                                 "WARNING: no definition for external symbol errno declared at file "))]},
            }
            records["replay"] = record(replay, es_tools, es, output)
        if args.with_verus:
            relative_source = "proofs/interlock.rs"
            if broken:
                proof_source = root / relative_source
                proof_source.write_text(proof_source.read_text().replace(
                    "fault || (!reset && previous_inhibit)", "!reset && previous_inhibit"))
            verus = ["verus", "--output-json", "--no-cheating", "--crate-type=lib", relative_source]
            result = subprocess.run(verus, cwd=root, capture_output=True, text=True, timeout=60)
            assert result.returncode == int(broken), result.stderr
            version = json.loads(result.stdout)["verus"]
            verus_tools = {"rustc":version["toolchain"], "cargo":tools["cargo"],"verus":"0.2026.11.02.8bf68ee"}
            plan["invocations"]["deductive"] = {
                "variant":"host", "command":verus, "toolchain":verus_tools,
                "checks":{"format":"verus","source":relative_source,
                          "function":"interlock::repeat_active_fault","verified_functions":3,
                          "requirement":"REQ-KEEP",
                          "proposition":"Across a positive number of resets with an active fault, the latch stays set."},
            }
            records["deductive"] = record(verus, verus_tools, result, result.stdout)
            records["deductive"]["diagnostics"] = result.stderr
        (root / "rust-verification.json").write_text(json.dumps(plan))
        inputs = json.loads(subprocess.check_output([safety, "rust", "snapshot", "--path", root]))
        bundle_file.write_text(json.dumps({"version":1,"producer":"local smoke test", "inputs":inputs,"invocations":records}))
        ingested = subprocess.run([safety,"rust","import","bundle.json","--path",root],capture_output=True,text=True)
        run = json.loads(ingested.stdout)
        expected_exit = 3 if broken and (args.with_esbmc or args.with_verus) else int(broken)
        assert ingested.returncode == expected_exit, run
        outcomes = {p["id"]:p["outcome"] for p in run["properties"]}
        expected = {"REQ-CLEAR@host":"PROVEN","REQ-KEEP@host":"VIOLATED" if broken else "PROVEN"}
        if args.with_esbmc:
            expected["REQ-CLEAR@host#esbmc"] = "UNKNOWN" if broken else "PROVEN"
            expected["REQ-KEEP@host#esbmc"] = "VIOLATED" if broken else "PROVEN"
        if args.with_verus:
            expected["deductive@host"] = "UNKNOWN" if broken else "PROVEN"
        assert outcomes == expected, outcomes
        if args.evidence_dir is not None:
            destination = args.evidence_dir / ("mutant" if broken else "reference")
            shutil.copytree(root / ".safety/runs" / run["id"], destination / "run")
            if args.with_esbmc:
                shutil.copy(root / model, destination / model)
        print(outcomes)
