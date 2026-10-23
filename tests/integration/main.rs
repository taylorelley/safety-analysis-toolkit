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
