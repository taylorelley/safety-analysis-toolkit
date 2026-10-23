# Executed validation and phase record

## Scope and commits

The implementation was delivered incrementally in commits `ae6d42a` (foundation
and Clang), `51cf372` (AST/ESBMC/coverage/MCP), `d5f1a75` (failure safeguards),
`6a14068` (persisted reports), `1c383de` (toolkit identity), and `eacb5f9`
(Frama-C, induction/harnesses, policy checks, per-build trace and evidence/process
completion). A follow-up guard rejects unreviewed compiler semantics in both
formal adapters (`d664e9a`) and includes a real regression fixture.
Trace index/link validation and per-build isolation tests are in `f71bb36`. The packaging/documentation and executed-evidence commits follow
that implementation in Git history. This file is recorded with those artifacts;
commit identifiers need not be self-referential.

The sole explicit specification change is the user's instruction:
“Proceed with unmapped checks; leave MISRA mappings unvalidated.” Therefore no
licensed guideline inventory, rule ID or overall compliance result is claimed.

## Commands and results

`validation/completion/full-validation.log` records the actual final native run:

- `cargo fmt --check` and `cargo clippy --locked --all-targets -- -D warnings`.
- Cargo unit and integration tests with real Clang/module/AST/MCP/coverage tools.
- CMake build against LLVM 19.1.7 and three CTest suites, including original
  policy rules and complex C++ AST cases.
- Release build and `scripts/validate.py`: both C/C++ examples analyze four
  entries and detect their two expected variant-specific null dereferences;
  native schemas, official content-pinned SARIF schema and hash inventories pass.
- Real ESBMC C/C++ safe/violated/insufficient-bound and induction/harness tests.
- Real Frama-C Eva valid/unknown and WP ACSL/Qed tests.

Combined direct development run: **24 unit tests and 17 integration tests passed**,
plus **three native CTest suites**. Optional analyzer tests are explicitly opted
in; ordinary Cargo runs show them as ignored, not passed. The final full command
separates native and formal suites for clear missing-tool behavior.

Additional actual commands and saved results:

- `python3 tests/integration/cli_cancel.py target/release/safety`:
  SIGINT cancels a native multi-unit scan, returns exit 3 and persists incomplete
  scope (`cli-cancellation.log`).
- `python3 scripts/configure_demo.py` explicitly builds and executes the known
  developer coverage fixture. The analysis engine never does this itself.
- `python3 scripts/e2e.py`: CLI and independent MCP workflows, Eva/WP, safe C
  harness, C++ counterexample, trace gap and known MC/DC gap. Native outputs are
  retained under `e2e/`; the deliberately seeded project remains incomplete.
- `python3 scripts/validate_harnesses.py`: all eight MCP tools through actual
  Codex 0.163.0 app-server and Pi 0.73.1 RPC/extension hosts, including discovery,
  findings, proofs, bounded counterexample retrieval, trace/coverage gaps and
  evidence-backed report. `harnesses/summary.json` records the host versions.
  These are deterministic real host transports, not mocked outputs or claims of
  model-driven judgment, independent verification or reviewer approval.
- `python3 scripts/benchmark.py`: seven independent small known-answer cases;
  zero missed seeded defects, zero safe-fixture false positives and zero
  incorrect overall proof classifications. Full observed results and durations
  are in `benchmark.json`; this small corpus is not a general accuracy estimate.
- `python3 scripts/archive_validation.py`: every archived run's hashes and
  native result schemas are checked; source/harness/configuration, instrumented
  executable and profile inputs are retained. Archive reports are reconstructed
  through the native CLI without analyzer execution.

`reference-validation.log` records fresh source/build/install and real tests in
the pinned reference container with network disabled and Cargo offline, followed
by original-artifact packaging, license aggregation and repeat-package hash
comparison. The final reference run starts from a fresh copy of the final
source candidate, including the compiler-semantics and trace-index guards.
Earlier candidate source updates are separately recorded as historical material.
Initial failed clean-install attempts are retained separately:
format/lint changes, disk exhaustion and a missing LLVM profiling runtime were
not represented as passes. The recipe now installs `libclang-rt-19-dev` and avoids
unnecessary filesystem layers. No external analyzer bundle is redistributed.

## Evidence replay and limitations

```sh
safety report validation/completion/e2e --format markdown
```

The archive contains original scopes and historical runs, including explicit
incomplete results. Each run retains its own configuration, source/tool hashes,
argv, model assumptions and native records. Historical `validation/e2e/` and
`validation/full-validation.log` remain unchanged records of the earlier slice;
the completion directory supersedes their capability assessment.

This host cannot enforce Landlock; strict mode fails closed. Seccomp networking
denial and cancellation are tested. The offline reference container supplies an
additional actual isolation boundary; no general sandbox claim is inferred for
fallback native execution. Hashes are integrity checks, not signatures.

All implementation phases have working real integrations. Remaining project
work is licensed MISRA mapping/inventory review (explicitly deferred), resolution
of intentional example findings/gaps, authorized assumption/deviation review,
target-specific verification and any applicable tool qualification. Those are
not automatic toolkit approvals. CI is configured; remote GitHub execution is
not claimed from this local environment. Broader platforms, proprietary flags,
dynamic call resolution and larger benchmark studies remain documented limits.
