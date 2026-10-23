use std::{fs, process::Command};
fn bin() -> Command {
    Command::new(env!("CARGO_BIN_EXE_safety"))
}
#[test]
fn init_does_not_overwrite() {
    let t = tempfile::tempdir().unwrap();
    assert!(
        bin()
            .args(["init", "--path"])
            .arg(t.path())
            .status()
            .unwrap()
            .success()
    );
    let first = fs::read(t.path().join("safety.yaml")).unwrap();
    assert_eq!(
        bin()
            .args(["init", "--path"])
            .arg(t.path())
            .output()
            .unwrap()
            .status
            .code(),
        Some(2)
    );
    assert_eq!(first, fs::read(t.path().join("safety.yaml")).unwrap());
}
#[test]
fn missing_database_is_explicit() {
    let t = tempfile::tempdir().unwrap();
    bin()
        .args(["init", "--path"])
        .arg(t.path())
        .output()
        .unwrap();
    let out = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(3));
    let v: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(v["complete"], false);
    assert_eq!(v["status"], "incomplete");
}
#[test]
fn invalid_config() {
    let t = tempfile::tempdir().unwrap();
    fs::write(
        t.path().join("safety.yaml"),
        "version: 2\nproject: {name: test}",
    )
    .unwrap();
    let out = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(2));
    let v: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(v["complete"], false);
}

fn fixture(cpp: bool) -> tempfile::TempDir {
    let t = tempfile::tempdir().unwrap();
    let ext = if cpp { "cpp" } else { "c" };
    fs::write(t.path().join("safety.yaml"),"version: 1\nproject: {name: known-answer}\nbuild: {compile_commands: compile_commands.json}\n").unwrap();
    fs::write(
        t.path().join(format!("unsafe.{ext}")),
        "int unsafe_value(void) { int *p=0; return *p; }\n",
    )
    .unwrap();
    fs::write(
        t.path().join(format!("safe.{ext}")),
        "int safe_value(int *p) { return p ? *p : 0; }\n",
    )
    .unwrap();
    let mut entries = Vec::new();
    for name in ["unsafe", "safe"] {
        for variant in [1, 2] {
            entries.push(serde_json::json!({"directory":t.path(),"file":format!("{name}.{ext}"),"arguments":[if cpp{"clang++-19"}else{"clang-19"},if cpp{"-std=c++17"}else{"-std=c11"},format!("-DVARIANT={variant}"),"-c",format!("{name}.{ext}")]}));
        }
    }
    fs::write(
        t.path().join("compile_commands.json"),
        serde_json::to_vec(&entries).unwrap(),
    )
    .unwrap();
    t
}
fn scan_fixture(cpp: bool) {
    let t = fixture(cpp);
    let before = fs::read(t.path().join(if cpp { "unsafe.cpp" } else { "unsafe.c" })).unwrap();
    let out = bin().arg("scan").arg(t.path()).output().unwrap();
    assert_eq!(
        out.status.code(),
        Some(1),
        "{}",
        String::from_utf8_lossy(&out.stdout)
    );
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["complete"], true);
    assert_eq!(r["completeness"]["total"], 4);
    assert_eq!(r["completeness"]["analyzed"], 4);
    let f = r["findings"].as_array().unwrap();
    assert_eq!(f.len(), 2);
    assert!(
        f.iter()
            .all(|v| v["rule_id"] == "clang-analyzer-core.NullDereference")
    );
    assert_ne!(f[0]["build_variant"], f[1]["build_variant"]);
    assert!(f.iter().all(|v| v["line"] == 1));
    assert_eq!(
        before,
        fs::read(t.path().join(if cpp { "unsafe.cpp" } else { "unsafe.c" })).unwrap()
    );
    let id = r["id"].as_str().unwrap();
    let report = bin()
        .arg("report")
        .arg(t.path())
        .args(["--run", id, "--format", "sarif"])
        .output()
        .unwrap();
    assert_eq!(report.status.code(), Some(1));
    let s: serde_json::Value = serde_json::from_slice(&report.stdout).unwrap();
    assert_eq!(s["version"], "2.1.0");
    assert_eq!(s["runs"][0]["results"].as_array().unwrap().len(), 2);
    fs::write(
        t.path().join(".safety/runs").join(id).join("findings.json"),
        "[]",
    )
    .unwrap();
    assert_eq!(
        bin()
            .arg("report")
            .arg(t.path())
            .args(["--run", id])
            .output()
            .unwrap()
            .status
            .code(),
        Some(2)
    );
}
#[test]
#[ignore = "requires real clang-tidy 19.1.7 on PATH"]
fn real_c_pipeline() {
    scan_fixture(false)
}
#[test]
#[ignore = "requires real clang-tidy 19.1.7 on PATH"]
fn real_cpp_pipeline() {
    scan_fixture(true)
}
#[test]
fn rejected_semantic_flag_is_incomplete() {
    let t = fixture(false);
    let p = t.path().join("compile_commands.json");
    let mut entries: serde_json::Value = serde_json::from_slice(&fs::read(&p).unwrap()).unwrap();
    for entry in entries.as_array_mut().unwrap() {
        entry["arguments"]
            .as_array_mut()
            .unwrap()
            .push("-fplugin=untrusted.so".into());
    }
    fs::write(p, serde_json::to_vec(&entries).unwrap()).unwrap();
    let out = bin().arg("scan").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(3));
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["complete"], false);
}
#[test]
#[ignore = "requires real clang-tidy 19.1.7 on PATH"]
fn compiler_failure_is_incomplete() {
    let t = fixture(false);
    fs::write(t.path().join("unsafe.c"), "int broken( { return 0; }").unwrap();
    let out = bin().arg("scan").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(3));
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["complete"], false);
    assert_eq!(r["completeness"]["failed"], 2);
}
#[test]
#[ignore = "requires real clang-tidy 19.1.7 and Python 3"]
fn real_mcp_stdio() {
    let t = fixture(false);
    let script =
        std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/integration/mcp_client.py");
    let out = Command::new("python3")
        .arg(script)
        .arg(env!("CARGO_BIN_EXE_safety"))
        .arg(t.path())
        .output()
        .unwrap();
    assert!(
        out.status.success(),
        "{}\n{}",
        String::from_utf8_lossy(&out.stdout),
        String::from_utf8_lossy(&out.stderr)
    );
}
#[test]
#[ignore = "requires real LLVM module and SAFETY_TEST_MODULE absolute path"]
fn real_custom_module() {
    let t = fixture(false);
    let module = std::env::var("SAFETY_TEST_MODULE").expect("set SAFETY_TEST_MODULE");
    let tidy = std::env::split_paths(&std::env::var_os("PATH").unwrap())
        .map(|p| p.join("clang-tidy-19"))
        .find(|p| p.is_file())
        .unwrap()
        .canonicalize()
        .unwrap();
    let config = serde_json::json!({"version":1,"project":{"name":"module"},"build":{"compile_commands":"compile_commands.json"},"tools":{"clang_module":module,"allowlist":[tidy,module]}});
    fs::write(t.path().join("safety.yaml"), config.to_string()).unwrap();
    fs::write(
        t.path().join("unsafe.c"),
        "int transfer(int x) { if(x) goto done; x=1; done: return x; }",
    )
    .unwrap();
    let out = bin().arg("scan").arg(t.path()).output().unwrap();
    assert_eq!(
        out.status.code(),
        Some(1),
        "{}",
        String::from_utf8_lossy(&out.stdout)
    );
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["complete"], true);
    assert_eq!(r["findings"].as_array().unwrap().len(), 2);
    assert!(
        r["findings"]
            .as_array()
            .unwrap()
            .iter()
            .all(|f| f["rule_id"] == "safety-prohibited-goto")
    );
}
#[test]
#[ignore = "requires real safety-source-index on PATH"]
fn real_ast_trace() {
    let t = fixture(true);
    fs::write(t.path().join("trace.yaml"),"version: 1\nnodes:\n- {id: R1, kind: requirement, root: true}\n- {id: T1, kind: test}\nlinks:\n- {from: R1, to: T1}\n").unwrap();
    fs::write(
        t.path().join("safe.cpp"),
        "/** @req R1 */\nint safe_value(int *p) { return p ? *p : 0; }\n",
    )
    .unwrap();
    fs::write(t.path().join("unsafe.cpp"),"/** @req MISSING */\nnamespace nav { template<class T> T value(T x) {return x;} int value(int x,int y){return x+y;} }\n").unwrap();
    let out = bin().arg("trace").arg(t.path()).output().unwrap();
    assert_eq!(
        out.status.code(),
        Some(1),
        "{}",
        String::from_utf8_lossy(&out.stdout)
    );
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["complete"], true);
    assert_eq!(r["completeness"]["analyzed"], 4);
    let f = r["findings"].as_array().unwrap();
    assert!(f.iter().any(|f| {
        f["message"]
            .as_str()
            .unwrap()
            .contains("Untraced code symbol")
    }));
    assert!(
        !f.iter()
            .any(|f| f["message"].as_str().unwrap().contains("R1 has no linked"))
    );
}
fn proof_fixture(cpp: bool, code: &str) -> tempfile::TempDir {
    let t = fixture(cpp);
    let file = if cpp { "unsafe.cpp" } else { "unsafe.c" };
    fs::write(t.path().join(file), code).unwrap();
    let db = serde_json::json!([{"directory":t.path(),"file":file,"arguments":[if cpp{"clang++-19"}else{"clang-19"},if cpp{"-std=c++17"}else{"-std=c11"},"-c",file]}]);
    fs::write(t.path().join("compile_commands.json"), db.to_string()).unwrap();
    t
}
#[test]
#[ignore = "requires real ESBMC 8.6.0 on PATH"]
fn real_esbmc_properties() {
    for cpp in [false, true] {
        for (code, exit, outcome) in [
            (
                "#include <assert.h>\nint main(void){int x=0;assert(x==0);return 0;}",
                0,
                "PROVEN",
            ),
            (
                "#include <assert.h>\nint main(void){int x=0;assert(x==1);return 0;}",
                1,
                "VIOLATED",
            ),
            (
                "#include <assert.h>\nint main(void){int x=0;for(int i=0;i<20;i++)x++;assert(x==20);return 0;}",
                3,
                "UNKNOWN",
            ),
        ] {
            let t = proof_fixture(cpp, code);
            let out = bin()
                .args([
                    "prove",
                    if cpp { "unsafe.cpp" } else { "unsafe.c" },
                    "--path",
                ])
                .arg(t.path())
                .args(["--engine", "esbmc", "--unwind", "2"])
                .output()
                .unwrap();
            assert_eq!(
                out.status.code(),
                Some(exit),
                "{}",
                String::from_utf8_lossy(&out.stdout)
            );
            let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
            assert!(
                r["properties"]
                    .as_array()
                    .unwrap()
                    .iter()
                    .any(|p| p["outcome"] == outcome)
            );
            let report = bin()
                .arg("report")
                .arg(t.path())
                .args(["--run", r["id"].as_str().unwrap(), "--format", "markdown"])
                .output()
                .unwrap();
            let text = String::from_utf8(report.stdout).unwrap();
            assert!(text.contains(outcome));
            assert!(text.contains("finite unwind 2"));
            if exit == 1 {
                let dir = t
                    .path()
                    .join(".safety/runs")
                    .join(r["id"].as_str().unwrap());
                assert!(dir.join("counterexamples.json").exists());
            }
            if exit == 0 {
                assert!(
                    r["properties"]
                        .as_array()
                        .unwrap()
                        .iter()
                        .all(|p| p["scope"].as_str().unwrap().contains("finite unwind 2"))
                );
            }
        }
    }
}
#[test]
#[ignore = "requires real LLVM/Clang 19.1.7 and Python 3"]
fn real_llvm_mcdc() {
    let script = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("tests/integration/coverage_fixture.py");
    let out = Command::new("python3")
        .arg(script)
        .arg(env!("CARGO_BIN_EXE_safety"))
        .output()
        .unwrap();
    assert!(
        out.status.success(),
        "{}\n{}",
        String::from_utf8_lossy(&out.stdout),
        String::from_utf8_lossy(&out.stderr)
    );
}
