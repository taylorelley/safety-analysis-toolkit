from pathlib import Path
"""Replay the known-answer interlock against Kani 0.68.0 and CBMC 6.11.0."""

import hashlib
import json
import shutil
import subprocess
import tempfile

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
            (root/"rust-verification.json").write_text(json.dumps({"version":1,"invocations":{"proof":plan_entry}}))
        else:
            assert plan_entry["command"] == checker
        inputs = json.loads(subprocess.check_output([safety,"rust","snapshot","--path",root]))
        record = {"command":plan_entry["command"],"toolchain":tools,"exit_code":result.returncode,
                  "output":result.stdout,"output_sha256":hashlib.sha256(result.stdout.encode()).hexdigest()}
        bundle_file.write_text(json.dumps({"version":1,"producer":"local smoke test", "inputs":inputs,"invocations":{"proof":record}}))
        ingested = subprocess.run([safety,"rust","import","bundle.json","--path",root],capture_output=True,text=True)
        run = json.loads(ingested.stdout)
        assert ingested.returncode == int(broken),run
        outcomes = {p["id"]:p["outcome"] for p in run["properties"]}
        assert outcomes == {"REQ-CLEAR@host":"PROVEN","REQ-KEEP@host":"VIOLATED" if broken else "PROVEN"},outcomes
        print(outcomes)
