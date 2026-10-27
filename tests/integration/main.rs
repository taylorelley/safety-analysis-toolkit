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
fn init_module_writes_reviewable_allowlist() {
    let t = tempfile::tempdir().unwrap();
    let missing = bin()
        .args(["init", "--path"])
        .arg(t.path())
        .arg("--module")
        .arg(t.path().join("absent.so"))
        .output()
        .unwrap();
    assert_ne!(missing.status.code(), Some(0));
    assert!(!t.path().join("safety.yaml").exists());
    let module = t.path().join("module.so");
    fs::write(&module, b"").unwrap();
    let out = bin()
        .args(["init", "--path"])
        .arg(t.path())
        .arg("--module")
        .arg(&module)
        .output()
        .unwrap();
    assert_eq!(out.status.code(), Some(0));
    let v: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert!(v["review"].as_str().unwrap().contains("review"));
    let config: serde_yaml::Value =
        serde_yaml::from_slice(&fs::read(t.path().join("safety.yaml")).unwrap()).unwrap();
    let module = module.canonicalize().unwrap();
    assert_eq!(config["tools"]["clang_module"].as_str(), module.to_str());
    let allowlist = config["tools"]["allowlist"].as_sequence().unwrap();
    assert_eq!(allowlist[0].as_str(), module.to_str());
    assert!(
        allowlist
            .iter()
            .all(|p| std::path::Path::new(p.as_str().unwrap()).is_absolute())
    );
    // The written configuration must load: doctor validates it on start-up.
    let doctor = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_ne!(
        doctor.status.code(),
        Some(2),
        "{}",
        String::from_utf8_lossy(&doctor.stderr)
    );
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
fn sandboxed_git_records_commit() {
    let t = tempfile::tempdir().unwrap();
    let git = |args: &[&str]| {
        Command::new("git")
            .args([
                "-c",
                "user.name=t",
                "-c",
                "user.email=t@t",
                "-c",
                "commit.gpgsign=false",
            ])
            .args(args)
            .current_dir(t.path())
            .output()
            .unwrap()
    };
    git(&["init", "-q"]);
    bin()
        .args(["init", "--path"])
        .arg(t.path())
        .output()
        .unwrap();
    git(&["add", "-A"]);
    git(&["commit", "-q", "-m", "fixture"]);
    let head = String::from_utf8(git(&["rev-parse", "HEAD"]).stdout).unwrap();
    let out = bin().arg("doctor").arg(t.path()).output().unwrap();
    let v: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(v["git"]["commit"], head.trim(), "{v}");
}
#[cfg(unix)]
fn stub_tool(dir: &std::path::Path, name: &str, script: &str) {
    use std::os::unix::fs::PermissionsExt;
    let path = dir.join(name);
    fs::write(&path, format!("#!/bin/sh\n{script}\n")).unwrap();
    fs::set_permissions(&path, fs::Permissions::from_mode(0o755)).unwrap();
}
#[cfg(unix)]
#[test]
fn doctor_reports_missing_analyzer_capabilities() {
    let t = tempfile::tempdir().unwrap();
    let stubs = tempfile::tempdir().unwrap();
    fs::write(t.path().join("x.c"), "int x;").unwrap();
    fs::write(
        t.path().join("compile_commands.json"),
        serde_json::json!([{"directory":t.path(),"file":"x.c","arguments":["cc","-c","x.c"]}])
            .to_string(),
    )
    .unwrap();
    fs::write(
        t.path().join("safety.yaml"),
        "version: 1\nproject: {name: caps}\nbuild: {compile_commands: compile_commands.json}\ntools: {clang: false, frama_c: true, esbmc: true}\n",
    )
    .unwrap();
    // Frama-C with Eva but without WP, as some distribution packages ship.
    stub_tool(
        stubs.path(),
        "frama-c",
        r#"case "$1" in -version) echo "25.0-beta (Manganese)";; -plugins) echo "Eva                 automatically computes";; esac"#,
    );
    let doctor = |solvers: &str| {
        stub_tool(
            stubs.path(),
            "esbmc",
            &format!(
                r#"case "$1" in --version) echo "ESBMC version 8.6.0 64-bit";; --list-solvers) echo "Available solvers: {solvers}" >&2;; esac"#
            ),
        );
        let path = format!(
            "{}:{}",
            stubs.path().display(),
            std::env::var("PATH").unwrap()
        );
        let out = bin()
            .arg("doctor")
            .arg(t.path())
            .env("PATH", path)
            .output()
            .unwrap();
        let v: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
        (out.status.code(), v)
    };
    let has = |v: &serde_json::Value, text: &str| {
        v["limitations"]
            .as_array()
            .unwrap()
            .iter()
            .any(|l| l.as_str().unwrap().contains(text))
    };
    let (code, v) = doctor("smtlib z3 boolector");
    assert!(has(&v, "frama-c: WP plugin unavailable"), "{v}");
    assert!(!has(&v, "Z3 solver unavailable"), "{v}");
    assert_eq!(
        (code, v["complete"].as_bool()),
        (Some(0), Some(true)),
        "{v}"
    );
    let (code, v) = doctor("smtlib boolector");
    assert!(has(&v, "esbmc: Z3 solver unavailable"), "{v}");
    assert_eq!(
        (code, v["complete"].as_bool()),
        (Some(3), Some(false)),
        "{v}"
    );
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
#[ignore = "requires real clang-tidy 19.1+ on PATH"]
fn real_c_pipeline() {
    scan_fixture(false)
}
#[test]
#[ignore = "requires real clang-tidy 19.1+ on PATH"]
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
#[ignore = "requires real clang-tidy 19.1+ on PATH"]
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
#[ignore = "requires real clang-tidy 19.1+ and Python 3"]
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
    let findings = r["findings"].as_array().unwrap();
    let count = |id: &str| findings.iter().filter(|f| f["rule_id"] == id).count();
    // One goto in each of the two compilation variants.
    assert_eq!(count("safety-prohibited-goto"), 2);
    assert_eq!(count("misra-c2012-rule-15-1"), 2);
    assert!(
        findings
            .iter()
            .all(|f| f["rule_id"].as_str().unwrap().starts_with("safety-")
                || f["rule_id"].as_str().unwrap().starts_with("misra-c2012-"))
    );
}
#[test]
#[ignore = "requires real safety-source-index on PATH"]
fn real_ast_trace() {
    let t = fixture(true);
    let config = serde_json::json!({"version":1,"project":{"name":"trace-variants"},"build":{"compile_commands":"compile_commands.json","variants":{"one":["VARIANT=1"],"two":["VARIANT=2"]}}});
    fs::write(t.path().join("safety.yaml"), config.to_string()).unwrap();
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
    // A link in one preprocessor build must not fill the other build's gap.
    fs::write(
        t.path().join("safe.cpp"),
        "#if VARIANT == 1\n/** @req R1 */\nint safe_value(int *p){return p ? *p : 0;}\n#else\nint safe_value(int *p){return p ? *p : 0;}\n#endif\n",
    )
    .unwrap();
    fs::write(t.path().join("unsafe.cpp"), "int other(void){return 0;}\n").unwrap();
    let out = bin().arg("trace").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(1));
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    let missing: Vec<_> = r["findings"]
        .as_array()
        .unwrap()
        .iter()
        .filter(|f| f["message"] == "Requirement R1 has no linked implementation")
        .collect();
    assert_eq!(missing.len(), 1, "{r}");
    assert_eq!(missing[0]["build_variant"], "two");
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
#[ignore = "requires real ESBMC 8.6.0+ on PATH"]
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
#[ignore = "requires real LLVM/Clang 19.1+ and Python 3"]
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
#[test]
#[ignore = "requires Frama-C 25.0+ and Clang 19.1+"]
fn real_frama_c() {
    for (code, method, entry, expected, success) in [
        (
            "#include <assert.h>\nint main(void){int x=1;assert(x==1);return 0;}",
            "eva",
            "main",
            "PROVEN",
            true,
        ),
        (
            "#include <assert.h>\nint main(void){int x=1;assert(x==2);return 0;}",
            "eva",
            "main",
            "UNKNOWN",
            false,
        ),
        (
            "/*@ assigns \\nothing; ensures \\result == x; */\nint identity(int x){return x;}",
            "wp",
            "identity",
            "PROVEN",
            true,
        ),
    ] {
        let t = proof_fixture(false, code);
        let out = bin()
            .args(["prove", "unsafe.c", "--path"])
            .arg(t.path())
            .args(["--engine", "frama-c", "--method", method, "--entry", entry])
            .output()
            .unwrap();
        assert_eq!(
            out.status.code(),
            Some(if success { 0 } else { 3 }),
            "{}",
            String::from_utf8_lossy(&out.stdout)
        );
        let value: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
        assert!(
            value["properties"]
                .as_array()
                .unwrap()
                .iter()
                .any(|p| p["outcome"] == expected),
            "{value}"
        );
        assert!(
            value["properties"]
                .as_array()
                .unwrap()
                .iter()
                .all(|p| p["scope"].as_str().unwrap().contains(entry))
        );
    }
}

#[test]
fn malformed_harness_and_oversized_config_rejected() {
    let t = proof_fixture(false, "int main(void){return 0;}");
    fs::write(
        t.path().join("harness.yaml"),
        "version: 1\nsource: unsafe.c\nanalyzer: arbitrary-shell-command",
    )
    .unwrap();
    let out = bin()
        .args(["prove", "unsafe.c", "--path"])
        .arg(t.path())
        .args(["--harness", "harness.yaml"])
        .output()
        .unwrap();
    assert_eq!(out.status.code(), Some(2));
    fs::write(t.path().join("safety.yaml"), vec![b' '; 1024 * 1024 + 1]).unwrap();
    let out = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(out.status.code(), Some(2));
    assert!(String::from_utf8_lossy(&out.stdout).contains("parser limit"));
}
#[test]
#[ignore = "requires real ESBMC 8.6.0+"]
fn real_esbmc_induction_harness() {
    let t = proof_fixture(
        false,
        "#include <assert.h>\nint main(void){int x=0;for(int i=0;i<5;i++)x++;assert(x==5);return 0;}",
    );
    let metadata = serde_json::json!({"version":1,"source":"unsafe.c","target":"main","property":"loop increments five times","input_assumptions":["sequential harness; fixed loop count"],"environment_model":"ESBMC sequential C model","build_configuration":"C11 64 bit","requirements":["TEST-REQ"],"analyzer":{"engine":"esbmc","method":"induction","entry":"main","bits":64,"unwind":8}});
    fs::write(t.path().join("harness.yaml"), metadata.to_string()).unwrap();
    let out = bin()
        .args(["prove", "unsafe.c", "--path"])
        .arg(t.path())
        .args([
            "--engine",
            "esbmc",
            "--method",
            "induction",
            "--harness",
            "harness.yaml",
        ])
        .output()
        .unwrap();
    assert_eq!(
        out.status.code(),
        Some(0),
        "{}",
        String::from_utf8_lossy(&out.stdout)
    );
    let r: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    assert_eq!(r["verification"]["request"]["method"], "induction");
    assert_eq!(r["verification"]["human_approval"], "not_granted_by_tool");
    for p in r["properties"].as_array().unwrap() {
        assert!(
            p["scope"]
                .as_str()
                .unwrap()
                .contains("induction search limit")
        );
        assert!(
            p["assumptions"]
                .as_array()
                .unwrap()
                .iter()
                .any(|a| a.as_str().unwrap().contains("User-supplied"))
        );
    }
    let mismatch = bin()
        .args(["prove", "unsafe.c", "--path"])
        .arg(t.path())
        .args([
            "--engine",
            "esbmc",
            "--method",
            "bmc",
            "--harness",
            "harness.yaml",
        ])
        .output()
        .unwrap();
    assert_eq!(mismatch.status.code(), Some(2));
}
#[test]
fn draft_misra_process_rejects_unreviewed_mapping_and_weakening() {
    let t = fixture(false);
    let mut config: serde_json::Value =
        serde_yaml::from_slice(&fs::read(t.path().join("safety.yaml")).unwrap()).unwrap();
    config["coding"] = serde_json::json!({"c":"MISRA-C:2012","registries":["registry.json"],"deviations":"deviations.json","recategorization":"grp.json"});
    fs::write(t.path().join("safety.yaml"), config.to_string()).unwrap();
    // Synthetic identifier tests process validation; it asserts no real MISRA mapping.
    let mut registry = serde_json::json!({"edition":"MISRA-C:2012","inventory_complete":false,"inventory_source":"synthetic process fixture, not a MISRA guideline","original_checks":[],"rules":[{"rule_id":"TEST-ONLY","coverage":"manual","implementation":null,"automated_checks":[],"manual_review_required":true,"limitations":["synthetic test"],"validation":null}]});
    fs::write(t.path().join("registry.json"), registry.to_string()).unwrap();
    fs::write(t.path().join("deviations.json"),"[{\"id\":\"DEV-1\",\"edition\":\"MISRA-C:2012\",\"rule_id\":\"TEST-ONLY\",\"rationale\":\"investigation pending\",\"artifacts\":[\"safe.c\"],\"change_request\":\"CR-1\"}]").unwrap();
    fs::write(t.path().join("grp.json"), "[]").unwrap();
    let good = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_ne!(
        good.status.code(),
        Some(2),
        "{}",
        String::from_utf8_lossy(&good.stdout)
    );
    let run: serde_json::Value = serde_json::from_slice(&good.stdout).unwrap();
    let draft: serde_json::Value = serde_json::from_slice(
        &fs::read(
            t.path()
                .join(".safety/runs")
                .join(run["id"].as_str().unwrap())
                .join("misra-drafts.json"),
        )
        .unwrap(),
    )
    .unwrap();
    assert_eq!(draft["deviations"]["approved_by_tool"], false);
    assert_eq!(draft["complete"], false);
    registry["rules"][0]["coverage"] = "supported".into();
    fs::write(t.path().join("registry.json"), registry.to_string()).unwrap();
    let bad = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(bad.status.code(), Some(2));
    registry["rules"][0]["coverage"] = "manual".into();
    fs::write(t.path().join("registry.json"), registry.to_string()).unwrap();
    fs::write(t.path().join("grp.json"),"[{\"edition\":\"MISRA-C:2012\",\"rule_id\":\"TEST-ONLY\",\"original_category\":\"mandatory\",\"proposed_category\":\"advisory\",\"rationale\":\"test weakening\"}]").unwrap();
    let bad = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(bad.status.code(), Some(2));
}

#[test]
fn bundled_misra_c_inventory_drafts_every_guideline_with_partial_automation() {
    let t = fixture(false);
    let mut config: serde_json::Value =
        serde_yaml::from_slice(&fs::read(t.path().join("safety.yaml")).unwrap()).unwrap();
    config["coding"] = serde_json::json!({"c":"MISRA-C:2012","recategorization":"grp.json"});
    fs::write(t.path().join("safety.yaml"), config.to_string()).unwrap();
    fs::write(t.path().join("grp.json"),"[{\"edition\":\"MISRA-C:2012\",\"rule_id\":\"Rule 15.1\",\"original_category\":\"advisory\",\"proposed_category\":\"required\",\"rationale\":\"project policy\"}]").unwrap();
    let out = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_ne!(
        out.status.code(),
        Some(2),
        "{}",
        String::from_utf8_lossy(&out.stdout)
    );
    let run: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
    let draft: serde_json::Value = serde_json::from_slice(
        &fs::read(
            t.path()
                .join(".safety/runs")
                .join(run["id"].as_str().unwrap())
                .join("misra-drafts.json"),
        )
        .unwrap(),
    )
    .unwrap();
    let guidelines = draft["gcs"]["guidelines"].as_array().unwrap();
    assert_eq!(guidelines.len(), 159);
    assert!(guidelines.iter().all(|g| {
        g["manual_review_required"] == true
            && g["compliance"]
                .as_str()
                .unwrap()
                .starts_with("not_established")
    }));
    assert_eq!(draft["gcs"]["inventory"][0]["inventory_complete"], true);
    let coverage = &draft["gcs"]["inventory"][0]["by_coverage"];
    assert_eq!(coverage["\"partial\""], 157);
    assert_eq!(coverage["\"manual\""], 2);
    // Mapped checks are listed, but nothing ran in doctor so execution is unproven.
    let rule = guidelines
        .iter()
        .find(|g| g["rule_id"] == "Rule 15.1")
        .unwrap();
    assert_eq!(rule["automated_checks"][0], "misra-c2012-rule-15-1");
    assert_eq!(rule["automated_execution"], "not_established");
    assert_eq!(draft["gcs"]["compliance"], "not_established");
    // Recorded categories reject a proposal that misstates the original category.
    fs::write(t.path().join("grp.json"),"[{\"edition\":\"MISRA-C:2012\",\"rule_id\":\"Rule 9.1\",\"original_category\":\"advisory\",\"proposed_category\":\"required\",\"rationale\":\"misstated\"}]").unwrap();
    let bad = bin().arg("doctor").arg(t.path()).output().unwrap();
    assert_eq!(bad.status.code(), Some(2));
}

#[test]
#[ignore = "requires Frama-C and ESBMC"]
fn real_frama_c_rejects_unreviewed_compiler() {
    let t = proof_fixture(
        false,
        "#include <assert.h>\nint main(void){assert(1);return 0;}",
    );
    let path = t.path().join("compile_commands.json");
    let mut entries: serde_json::Value = serde_json::from_slice(&fs::read(&path).unwrap()).unwrap();
    entries[0]["arguments"][0] = "unreviewed-proprietary-cc".into();
    fs::write(path, entries.to_string()).unwrap();
    for engine in ["esbmc", "frama-c"] {
        let out = bin()
            .args(["prove", "unsafe.c", "--path"])
            .arg(t.path())
            .args(["--engine", engine])
            .output()
            .unwrap();
        assert_eq!(
            out.status.code(),
            Some(3),
            "{}",
            String::from_utf8_lossy(&out.stdout)
        );
        let run: serde_json::Value = serde_json::from_slice(&out.stdout).unwrap();
        assert_eq!(run["completeness"]["unsupported"], 1);
        assert!(
            run["properties"]
                .as_array()
                .unwrap()
                .iter()
                .all(|p| p["outcome"] != "PROVEN")
        );
    }
}
