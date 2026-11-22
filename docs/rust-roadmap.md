# Rust support and agentic-development plan

Status: **proposed plan**. Nothing described here is implemented yet. It
extends the toolkit from C/C++ to Rust applications and adds controls aimed at
code produced by AI coding agents. The existing guarantees remain binding: no
compliance or certification claim, no automatic source changes, no reused
cached evidence, no hidden unsupported scope, and agent reasoning stays
advisory.

Some upstream details move quickly and must be re-confirmed when each phase
starts (marked **verify** below): Rust coverage instrumentation options, the
qualification scope of Ferrocene, the status of MISRA's Rust applicability
addendum and of the Safety-Critical Rust Consortium coding guidelines, and the
stability of rustdoc JSON output.

## 1. Goals and non-goals

Goals:

- Analyse Rust crates and workspaces with the same evidence model
  (`.safety/runs/RUN-*`, SHA-256 inventory, completeness, provenance) as C/C++.
- Support mixed C/C++ and Rust projects, including FFI boundaries.
- Give agents fast, bounded, stable feedback while keeping evidence runs strict.
- Make the failure modes typical of agent-written code visible: suppressed
  lints, unjustified `unsafe`, new panic paths, unreviewed dependencies and
  edits to analysis configuration.

Non-goals:

- New analysis engines. Rust work wraps rustc/Clippy, Kani, Miri,
  `llvm-cov`, cargo-deny/audit/vet and rustdoc/rust-analyzer output.
- Applying fixes. Machine-applicable suggestions are exposed as data only.
- Claiming guideline compliance. Rust guideline mappings are partial and await
  independent review, exactly like the MISRA C:2012 checks.

## 2. Guiding decisions

1. **One binary, language adapters.** Rust is a second adapter family beside
   Clang/Frama-C/ESBMC, sharing `runner`, `evidence`, `findings`, `report`,
   `mcp` and `trace` graph validation.
2. **Import first, execute by authorization.** Cargo, Clippy and rust-analyzer
   run `build.rs` and procedural macros, which is arbitrary project code. This
   conflicts with the rule that the engine never executes project build
   scripts. The default Rust mode therefore imports diagnostics produced
   elsewhere (as `coverage import` already does). Executing Cargo is a separate,
   explicitly reviewed opt-in (section 5).
3. **Pinned toolchain as an input.** A Rust run records the toolchain as
   strictly as LLVM 19.1 is recorded today. An unpinned or unrecognised
   toolchain is a recorded limitation; a mismatch with the configured reference
   makes the run incomplete.
4. **Evidence runs vs advisory runs.** Agents need a fast inner loop; evidence
   must stay uncached and complete. These become two distinct, labelled run
   kinds (section 9.3).

## 3. Phase 0: language-neutral refactor (prerequisite)

No behaviour change for C/C++; all existing tests and validation must pass
unchanged.

| Work item | Files | Detail |
| --- | --- | --- |
| Build-variant abstraction | `src/discovery.rs`, `src/adapters/*.rs` | Introduce `enum Unit { C(CUnit), Rust(RustUnit) }` (or a trait) where `CUnit` is the current `Unit` struct. Adapters match on the variant they support and report others as `unsupported`. |
| Finding language field | `src/model.rs`, `schemas/findings.schema.json` | Add `language: Option<String>` (`c`, `cpp`, `rust`) with serde default; bump `schema_version` to 2 and keep reading version 1. |
| Stable fingerprints | `src/findings.rs`, `src/model.rs` | Add `fingerprint` = SHA-256 of tool, rule, language, symbol path (USR or Rust item path) and normalised span text, excluding line numbers. Prerequisite for run comparison (9.2). Applies to C/C++ as well. |
| Config layout | `src/config.rs`, `schemas/project.schema.json` | Add `project.languages` (default `[c, cpp]`) and an optional `rust:` section (section 4). Keep `deny_unknown_fields`. Config `version` stays 1; new keys are optional. |
| Operation registry | `src/main.rs`, `src/mcp.rs` | Route `scan`/`trace`/`prove`/`coverage` per configured language, so a mixed project produces one run per operation containing all languages with per-language completeness counters. |

Acceptance: `make validation` passes; JSON output for existing examples is
identical apart from the new optional fields; schema tests cover v1 and v2.

## 4. Phase 1: Rust discovery and toolchain provenance

### 4.1 Configuration

```yaml
project:
  name: example
  languages: [rust]          # or [c, rust] for mixed projects
rust:
  manifest: Cargo.toml       # workspace or package manifest, inside the root
  mode: import               # import | execute (section 5)
  toolchain:
    reference: "1.xx.y"      # exact rustc release that is validated for this project
    channel: stable          # stable | ferrocene | nightly-YYYY-MM-DD
  variants:                  # explicit, reviewed variant matrix
    host-debug:   {profile: dev,     target: x86_64-unknown-linux-gnu, features: [],        no_default_features: false}
    target-rel:   {profile: release, target: thumbv7em-none-eabihf,   features: [hw-v2],   no_default_features: true}
  imports:                   # used in import mode
    clippy: build/clippy.jsonl
    provenance: build/rust-provenance.json
```

### 4.2 Discovery

- Read workspace structure from `cargo metadata --format-version 1 --locked
  --offline --no-deps`. `--no-deps` metadata does not run build scripts, but
  it is still Cargo execution: run it through `runner::execute` with the
  cleared environment, no network and an allowlisted `cargo`. As a fallback that
  executes nothing, parse `Cargo.toml` and `Cargo.lock` directly.
- `RustUnit` = package id + target name and kind (lib, bin, test, bench,
  example, proc-macro, build-script) + variant (profile, target triple, feature
  set, `cfg` set) + edition + manifest path.
- Every configured variant is a separate build variant, mirroring how distinct
  compile commands are kept today. Feature combinations that are not configured
  are **out of scope and listed** in limitations, never silently assumed
  covered.
- Flag `build.rs` and proc-macro crates in discovery output: they determine
  whether import mode is the only safe option.
- Reject unreviewed `RUSTFLAGS`, `[target.*.rustflags]`, `[build] rustflags`,
  linker overrides and `[patch]`/`[replace]` sections unless listed in a reviewed
  vocabulary, analogous to `validate_args`. Unknown flags make the unit
  `unsupported`, not silently dropped.

### 4.3 Provenance

Record in the run's `inputs` and `toolkit` fields:

- `rustc -vV`, `cargo -V`, `clippy-driver -V` output and executable hashes.
- `rust-toolchain.toml`, `Cargo.lock`, every `Cargo.toml`, `.cargo/config.toml`,
  `clippy.toml`, `deny.toml` and `supply-chain/` hashes.
- Vendored source directory hash (when `vendor/` or a configured directory is
  used) so that dependency source is fingerprinted, which C/C++ headers outside
  the workspace currently are not.
- Toolchain reference comparison, using the same pattern as
  `discovery::reference_note`.

`safety doctor` gains Rust checks: cargo/rustc/clippy presence and versions,
`llvm-tools` component and its LLVM version, Kani, Miri, cargo-deny, cargo-audit
and cargo-vet availability, and whether the configured mode is possible.

Acceptance: a new `examples/embedded-rust` workspace (one `no_std` library,
one host binary, one deliberately failing case per Rust check family, two
variants) produces deterministic discovery output in a unit test and in
`safety doctor`.

## 5. Phase 1: build-script execution policy

| Mode | What runs | When to use | Recorded as |
| --- | --- | --- | --- |
| `import` (default) | Nothing from the project. The toolkit parses rustc/Clippy JSON produced by CI or a developer, plus a provenance file with hashes of the inputs and the exact command line. | Untrusted repositories, agent sessions, any project with `build.rs` or proc macros. | Diagnostics provenance is a supplied assertion, verified against hashes like coverage provenance. |
| `execute` | `cargo clippy --locked --offline --frozen --message-format=json` with an explicit `--target`, `--profile` and feature set per variant, through `runner::execute`. | Reviewed repositories where the user accepts that `build.rs` and proc macros run. | Requires `rust.allow_build_scripts: true` in the reviewed `safety.yaml`. Every run records that project code executed. |

Execute-mode constraints:

- Seccomp network denial and the cleared environment already provided by the
  runner; `CARGO_HOME` and `CARGO_TARGET_DIR` inside the run's evidence
  directory or a configured scratch directory; vendored dependencies required.
- Landlock write restriction to the target directory; when
  `tools.require_filesystem_isolation` is true and Landlock is unavailable the
  run is refused, as for analyzers today.
- Per-variant time, memory and output limits from `tools`.
- Never `cargo build` of the final artifact, never `cargo test` or `cargo run`
  (test execution keeps its separate authorization, section 7.2).

Acceptance: integration test proving that import mode executes no Cargo
subprocess besides optional metadata; a fixture `build.rs` that attempts a
network connection fails under execute mode and the run records the failure.

## 6. Phase 1: static analysis adapter (`src/adapters/rust.rs`)

### 6.1 Diagnostics

- Parse rustc JSON diagnostics (`reason: compiler-message`) line by line with a
  bounded reader. Map `code.code` to `rule_id` (`clippy::unwrap_used`,
  `rustc::unused_must_use`, `E0599`), primary span to file/line/column/end, the
  macro expansion chain to `native_references`, and `level` to severity.
- Retain the raw JSONL byte for byte in the run directory.
- Diagnostics in dependency crates or outside the workspace are recorded but
  excluded from findings with an explicit count, never dropped silently.
- Malformed JSON, a missing `build-finished` record, a non-zero exit without
  diagnostics, or a compile error in a configured variant all make the unit
  incomplete.
- `children` suggestions with `MachineApplicable` applicability become a
  `suggestions` array on the finding (span plus replacement text, size bounded).
  They are data; the toolkit never writes them (section 9.4).

### 6.2 Rust safety profile (`profiles/rust-safety.yaml`)

A pinned, reviewed lint selection, applied with explicit `-D`/`-W` arguments
so that project `[lints]` tables cannot weaken it, just as project
`.clang-tidy` is overridden today. Initial families:

| Family | Example lints | Rationale |
| --- | --- | --- |
| Panics | `clippy::unwrap_used`, `expect_used`, `panic`, `todo`, `unimplemented`, `unreachable`, `indexing_slicing`, `get_unwrap` | Panic paths are the main Rust analogue of undefined behaviour in safety arguments |
| Arithmetic and conversion | `clippy::arithmetic_side_effects`, `as_conversions`, `cast_possible_truncation`, `cast_sign_loss`, `float_arithmetic` (optional) | Overflow behaviour differs between debug and release |
| `unsafe` hygiene | `unsafe_op_in_unsafe_fn`, `clippy::undocumented_unsafe_blocks`, `multiple_unsafe_ops_per_block`, `missing_safety_doc` | Each `unsafe` operation needs a local justification |
| Suppression hygiene | `clippy::allow_attributes`, `allow_attributes_without_reason` | Required by section 9.1 |
| Resource and concurrency | `clippy::mem_forget`, `rc_buffer`, `await_holding_lock`, `std_instead_of_core` (for `no_std`) | Determinism and embedded constraints |
| Hygiene | `missing_docs`, `rustc::unused_must_use`, `clippy::exit` | Traceability and reviewability |

The profile records, for each lint, the Clippy version in which it was
validated. A missing lint in the installed Clippy is an unsupported check, not a
pass.

### 6.3 Guideline registries

- Add `profiles/rust-guidelines.yaml` modelled on `profiles/misra-c-2012.yaml`:
  identifiers and classifications only, sourced from a licensed copy supplied
  by the user for MISRA's Rust applicability addendum (**verify** title and
  edition), and from the Safety-Critical Rust Consortium coding guidelines
  (**verify** status and licence).
- Each guideline maps to zero or more lints, an inventory (section 6.4) or
  `manual`. Coverage documentation mirrors `docs/misra-coverage.md` and states
  that mappings are partial and await independent review.
- Custom lints that Clippy lacks are a later item (phase 5), built with Dylint
  or a `rustc_private` driver pinned to one nightly, with known-answer fixtures
  like `clang/tests/misra/`.

### 6.4 Inventories

Inventories are findings plus a structured artefact in the run directory.

- **`unsafe` inventory** (`unsafe.json`): every `unsafe` block, `unsafe fn`,
  `unsafe impl`, `unsafe trait`, `extern` block, `#[no_mangle]`/`#[export_name]`
  item, `static mut` and `union`, with item path, span, the `// SAFETY:`
  comment text, the requirement/design links (section 8) and the crate it
  belongs to. Extracted from rustdoc JSON or a syntax pass with `syn` (no
  macro expansion; macro-generated `unsafe` is reported as an unresolved gap).
  Unjustified or untraced `unsafe` is a finding.
- **Panic-path inventory** (`panics.json`): aggregated panic-family
  diagnostics per item, so reports can state "N panic sites in safety-scoped
  crates" per variant.
- **Suppression inventory** (`suppressions.json`): every `#[allow]`,
  `#[expect]`, `#![allow]`, `[lints]` table entry, `clippy.toml` setting and
  `cfg_attr(..., allow(...))`, with reason text. Feeds section 9.1.

Acceptance: `examples/embedded-rust` produces a known set of findings per
variant (documented like the C examples), raw output is retained, SARIF output
validates against the cached schema, and an import with a tampered input hash
is rejected.

## 7. Phase 2: coverage and supply chain

### 7.1 Coverage import

- Reuse `src/coverage.rs` for Rust binaries built with
  `-C instrument-coverage`. The `llvm-cov`/`llvm-profdata` executables must
  come from the same LLVM major release as the rustc that built the binary (the
  `llvm-tools` component), not the system LLVM 19.1. Doctor compares the
  `rustc -vV` LLVM version with the `llvm-cov --version` LLVM version, and a
  mismatch makes the import incomplete.
- Make the accepted `llvm-cov export` JSON versions a per-toolchain table, not
  the single 2.0.1 value.
- Branch coverage for Rust is nightly-only and MC/DC is not currently
  available (**verify** for the pinned toolchain). The importer reports
  missing measurement kinds as **not measured**, which keeps the import
  incomplete when the configured assurance level requires them. It never
  substitutes line coverage.
- Extend coverage provenance with `rustc_version`, `rustflags`, `features`,
  `target`, `profile` and `cargo_lock_sha256`.
- Demangle Rust symbols (v0 and legacy) for gap reporting.

### 7.2 Dynamic checks under test-execution authorization

Miri, `cargo test` and sanitizer runs execute project code. They stay outside
the default engine, matching the current rule that the engine never executes
tests. Two options, in order of preference:

1. **Import**: CI runs `cargo miri test` and `cargo test` and supplies
   results and logs with provenance; the toolkit records them as distinct
   evidence (`miri` tool, per-test outcome, UB reports as findings).
2. **Authorized execution** (later): `safety miri --authorize-test-execution`
   with the same isolation as section 5, recorded as test execution.

### 7.3 Supply-chain evidence

- `cargo deny check --format json` (licences, bans, sources, advisories)
  imported or executed. It does not run build scripts; execution is allowed with
  `--offline` against a vendored advisory database whose hash is recorded.
- `cargo audit --json` against a pinned advisory database snapshot (hash recorded).
- `cargo vet` status: unaudited crates are findings; the audit files are
  hashed inputs, never edited by the toolkit.
- Dependency inventory per variant (resolved features included) as
  `dependencies.json`, joined to the run.
- New dependency or version since the configured baseline run is a finding of
  category `supply-chain` (section 9.2).

Acceptance: a fixture with one advisory crate, one disallowed licence and one
unvetted crate yields exactly those findings; coverage import of a known Rust
binary reproduces a documented line/region gap and reports branch/MC/DC as not
measured.

## 8. Phase 3: formal verification

### 8.1 Kani adapter (`src/adapters/kani.rs`)

- `safety prove --engine kani --harness-fn crate::module::harness --variant VAR`.
- Runs `cargo kani --harness <name> --output-format terse`, or imports its
  output in import mode. Kani compiles the crate, so it falls under the section 5
  execution policy.
- Map each Kani check to `Property` with `ProofOutcome`:
  `SUCCESS` → `PROVEN` (scoped to the recorded unwind bound, solver and harness
  assumptions), `FAILURE` → `VIOLATED` with the counterexample stored in
  `counterexamples.json`, `UNREACHABLE` → `NOT_CHECKED` with a note,
  `UNDETERMINED` or a timeout → `UNKNOWN`, and an unwinding assertion failure →
  `UNKNOWN` with a hint (as `esbmc::unwind_hint` does).
- Record `#[kani::unwind]`, `#[kani::solver]`, `kani::assume` calls and
  `#[kani::stub]` substitutions as assumptions. Stubs are model changes and must
  appear in `scope`.
- Reuse `verification::Request` validation and the existing `--harness`
  metadata file. Agent-drafted harnesses are welcome, but their assumptions
  remain flagged as requiring authorized human review.
- Function contracts (`#[kani::requires]`/`ensures`) are recorded with their
  proof mode; contract-replacement proofs are reported as compositional and
  depending on the replaced contracts being proven separately.

### 8.2 Further engines (evaluation only)

Verus, Creusot and Prusti are evaluated after Kani for deductive proofs of
specific modules. Each needs a pinned toolchain and its own outcome mapping;
none is adopted without known-answer fixtures. ESBMC's Rust support is also
evaluated (**verify** maturity) because it would reuse the existing adapter.

Acceptance: real Kani run on `examples/embedded-rust` with one proven harness,
one violated harness with a stored counterexample and one bounded harness that
reports `UNKNOWN` when the unwind bound is too small, all gated behind an
opt-in `make full-validation` step like ESBMC/Frama-C.

## 9. Phase 1-2: controls for agentic development

These apply to C/C++ as well; they are scheduled early because agents write
Rust quickly and pressure to make diagnostics disappear is high.

### 9.1 Suppressions are deviations

- Every suppression in the inventory (6.4) that is not covered by an entry in
  the reviewed deviations file (`coding.deviations`) is a finding of category
  `suppression`, severity at least that of the suppressed lint.
- Changes to `safety.yaml`, `Cargo.toml` `[lints]`, `clippy.toml`,
  `rust-toolchain.toml`, `.cargo/config.toml`, `deny.toml`, `supply-chain/`
  and (for C/C++) `.clang-tidy` and the compilation database are listed in the
  report as **analysis-configuration changes since baseline**, because an agent
  can weaken analysis there instead of fixing code.
- `#[cfg(...)]` gating that removes code from every configured variant is
  reported (code that no variant compiles is unanalysed).

### 9.2 Baselines and run comparison

- New command `safety compare --base RUN-A --head RUN-B` and MCP tool
  `safety.compare_runs`. Both runs are integrity-checked first.
- Uses fingerprints (phase 0) to classify findings as `new`, `resolved`,
  `unchanged` and `moved`; compares completeness, executions, toolchain,
  configuration hashes, inventories (`unsafe`, panics, suppressions,
  dependencies) and proof outcomes (a `PROVEN` property becoming `UNKNOWN` is a
  regression).
- Optional reviewed ratchet policy in `safety.yaml`:

  ```yaml
  gates:
    baseline_run: RUN-...        # or a git ref resolved to the latest evidence run for it
    new_findings: fail
    new_unsafe: fail
    new_suppressions: fail
    new_dependencies: fail
    completeness_regression: fail
  ```

  A gate failure is exit 1 with findings, never a silent pass. Gates cannot be
  configured through MCP.

### 9.3 Advisory runs for the inner loop

- `safety check [--changed REF]` and MCP `safety.check`: restrict to units,
  crates or items affected by a git diff (using the trace graph and Cargo's
  dependency graph), reuse the compiler output that already exists in import
  mode, and stop on the first configurable number of findings.
- Results are written under `.safety/advisory/` with `run_kind: advisory`, are
  never eligible for `safety report` evidence output, and are labelled
  "advisory, not evidence" in every format. Evidence runs (`scan`, `trace`,
  `prove`, `coverage import`) are unchanged and still reuse nothing.
- Target: under 5 seconds of toolkit overhead on the example workspace,
  excluding compiler time.

### 9.4 Fix suggestions as data

- Expose `suggestions` (6.1) in compact `safety.get_findings` records,
  bounded per finding. Agents apply them with their own editing tools within
  authorized scope; the toolkit has no source-write method and gains none.
- The report states, for each resolved finding in a comparison, whether the
  change was a code change or a suppression/configuration change.

### 9.5 Authorship and review provenance

- Record in run `git` metadata: commit trailers (`Co-Authored-By`, agent
  session links), the authors of changed lines since the baseline (from `git
  log`, read-only), and an optional `SAFETY_AGENT_SESSION` value passed by the
  host (allowlisted environment variable, recorded, not trusted as identity).
- Reports list agent-attributed changes in safety-scoped code that lack an
  imported independent human review record (`evidence.reviews`). This is an
  outstanding-work item, never an approval.

### 9.6 MCP additions

All follow existing rules: fixed workspace, strict schemas, bounded results,
cancellation, no shell, no source writes.

| Tool | Purpose |
| --- | --- |
| `safety.check` | Advisory scoped run (9.3) |
| `safety.compare_runs` | Run diff (9.2), paged |
| `safety.unsafe_inventory` | Paged `unsafe` inventory with justification and trace status |
| `safety.dependencies` | Paged dependency and supply-chain status |
| `safety.prove` | Extended with `engine: kani` |
| `safety.explain_rule` | Toolkit-authored description of a lint or check, its mapping status and known limitations; never MISRA text |

`safety.get_findings` gains `language`, `fingerprint`, `since_run` and
`suggestions` filters/fields.

### 9.7 Skill and host integration

- `skills/safety-analysis/SKILL.md`: add a Rust section covering import vs
  execute mode, "Kani `UNKNOWN` and unwinding failures are not proof",
  "Miri and tests need test-execution authorization", "never add `allow`,
  `expect`, `cfg` gating or configuration changes to remove a finding without a
  reviewed deviation", "do not add dependencies outside the authorized change
  scope", and "use `safety.check` while iterating and an evidence run before
  reporting".
- `safety init --lang rust` writes a draft `safety.yaml`, a `[workspace.lints]`
  table matching the profile, `deny.toml`, a `rust-toolchain.toml` pin
  suggestion and an AGENTS.md section. All drafts are for human review.
- Example host hooks in `examples/agent-harnesses/` (Claude Code, Codex): run
  `safety check --changed HEAD` after edits and surface new findings. Hooks are
  examples, not dependencies.

## 10. Phase 4: Rust traceability

- Symbol source: rustdoc JSON (`--output-format json`, nightly-only,
  **verify** stability; pin one nightly and the `format_version`) as the
  primary source, `rust-analyzer scip` as an alternative. Both run proc macros
  and build scripts, so section 5 applies; import mode accepts their output
  with provenance.
- Symbol identity: canonical item path plus crate id and version
  (`crate@1.2.3::module::Type::method`), with impl blocks disambiguated by
  self type and trait. Stored in `trace::Symbol` alongside USRs; the manifest
  `code` node id accepts either form.
- Annotations: `/// @req R1`, `/// @design D1`, `/// @test T1`,
  `/// @baseline BL-1` in doc comments, using the existing `tokens` parser.
  `#[test]`, `#[kani::proof]` and integration-test functions are identified
  as tests automatically, and still need `@test` ids to link to requirements.
- Calls: direct and static method calls from rust-analyzer call hierarchy or
  SCIP references. Trait-object dispatch, generic monomorphisation, closures and
  macro-generated items are reported as explicit unresolved gaps, as virtual
  calls and lambdas are for C++.
- FFI boundary: link Rust `extern "C"` declarations and `#[no_mangle]`
  exports to C/C++ USRs by symbol name and recorded signature; signature
  mismatches between the bindgen output or the declaration and the C prototype
  are findings. Every `unsafe` FFI call site appears in the `unsafe` inventory
  with its trace links.

Acceptance: a mixed C and Rust fixture where a requirement is traced from the
manifest through a Rust function and an FFI call into a C function, with one
deliberate signature mismatch and one untraced `unsafe` block reported.

## 11. Phase 5: hardening

- Custom lints (Dylint or a pinned `rustc_private` driver) for guideline
  mappings Clippy lacks, with known-answer fixtures and CI on the pinned
  nightly.
- Execute-mode hardening: fuzz the JSON parsers (Clippy, Kani, cargo-deny,
  rustdoc) with the existing bounded readers; aggregate memory limits for
  Cargo's process tree.
- Ferrocene as a first-class reference toolchain: doctor recognises its
  version string; documentation records which qualification artefacts the
  project must obtain from the vendor (**verify** scope). The toolkit itself
  claims no tool qualification.
- AArch64 and embedded target validation (`thumbv7em-none-eabihf`,
  `aarch64-unknown-none`) for import mode.

## 12. Validation additions

| Item | Make target | Notes |
| --- | --- | --- |
| `examples/embedded-rust` known-answer workspace | `make validation` | Documented expected findings per variant, like the C examples |
| Mixed C/Rust FFI fixture | `make validation` | Trace and `unsafe` inventory joins |
| Import-mode tests (no Cargo execution) | `make test` | Asserts no subprocess besides allowlisted metadata |
| Execute-mode tests with hostile `build.rs` | `make full-validation` | Network and write attempts fail and are recorded |
| Real Clippy run | `make validation` (ignored unless requested) | Same pattern as real-Clang tests |
| Real Kani, Miri import, cargo-deny/audit/vet | `make full-validation` | Opt-in; a missing tool is skipped, never passed |
| Rust coverage import | `make validation` | Toolchain-matched `llvm-cov` |
| MCP client tests for new tools | `make test` | Extend `tests/integration/mcp_client.py` |
| Schema v2 compatibility | `make check` | Old evidence still loads and verifies |

`docs/tool-versions.json` gains Rust entries (toolchain reference, Clippy,
Kani, cargo-deny, cargo-audit, cargo-vet, advisory database snapshot hash) and
`scripts/bootstrap.sh` installs them when requested with a flag.

## 13. Sequencing and dependencies

```
Phase 0 refactor ──┬─> Phase 1 discovery + provenance ─> Phase 1 Clippy adapter + inventories
                   │                                   └─> Phase 2 coverage + supply chain
                   └─> 9.1/9.2 suppressions, fingerprints, compare_runs (C/C++ first)
Phase 1 adapter ──────> 9.3 advisory runs ─> 9.6 MCP tools ─> 9.7 skill and hooks
Phase 1 execute mode ─> Phase 3 Kani ─> Phase 4 traceability (rustdoc/SCIP) ─> Phase 5
```

Rough size, as a guide only: phase 0 small; phase 1 the largest (discovery,
policy, adapter, inventories, example); phase 2 medium; 9.x medium and largely
language-neutral; phase 3 medium; phase 4 large because of symbol identity and
FFI joins; phase 5 open-ended.

## 14. Risks and open decisions

| Risk or decision | Mitigation or proposal |
| --- | --- |
| Build scripts and proc macros execute project code | Import mode by default; execute mode only with a reviewed flag, isolation and recording (section 5) |
| Toolchain churn breaks parsers (rustdoc JSON, Clippy lint names, coverage JSON) | Pin per toolchain, version tables, unknown versions are incomplete, never best-effort |
| Feature-combination explosion | Only configured variants are in scope; unconfigured combinations are listed as limitations |
| Rust MC/DC unavailable | Report as not measured; do not substitute branch or line coverage |
| Fingerprint instability across refactors | Include symbol path, not lines; report `moved` separately; never auto-dispose |
| Advisory runs mistaken for evidence | Separate directory, `run_kind`, label in every format, `report` refuses them |
| Guideline licensing | Identifiers and classifications only from user-supplied licensed copies; toolkit text only in `explain_rule` |
| Agent edits to analysis configuration | Configuration-change reporting and gates that cannot be set through MCP (9.1, 9.2) |
| Decision: rustdoc JSON vs rust-analyzer SCIP for traceability | Prototype both on the example in phase 4 and choose by identity stability and gap reporting |
| Decision: Dylint vs `rustc_private` driver for custom lints | Decide in phase 5; Dylint preferred if it can be pinned and loaded without network access |

## 15. Documentation changes per phase

Each phase updates `README.md` (scope sentence), `docs/architecture.md`,
`docs/cli.md`, `docs/mcp.md`, `docs/development-status.md`,
`docs/dependencies.md`, `docs/installation.md`, `THIRD_PARTY_NOTICES.md` where
new Rust crates are added (for example `syn` or `rustc-demangle`), and adds
`docs/rust.md` and `docs/rust-guideline-coverage.md` once the corresponding
work exists. Capability-boundary language follows the existing documents: what
was executed and validated, and what remains outside the claim.
