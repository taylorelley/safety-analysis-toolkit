# Rust proof and diagnostic import

The import path accepts a workspace verification plan and results collected by a
separate producer. It records diagnostic findings and proof properties through
the existing run, report and MCP interfaces.

## Prepare inputs

Put the reviewed plan in `rust-verification.json` beside `safety.yaml`. For
each intended Clippy or proof execution, assign an invocation ID and record the
variant, exact command (including target and features) and toolchain identities.
Keep the approved plan in a protected revision and compare it independently
when preparing CI evidence. This importer checks the plan in the supplied
workspace. It has no mechanism to authenticate an approval or producer.

Run `safety rust snapshot --path WORKSPACE` before placing the bundle in the
workspace. Put compiler outputs and Cargo's target directory outside that
workspace during collection. The snapshot covers the workspace inventory,
including source, lockfile, plan and configuration. The ordinary evidence
directory and Git metadata are excluded. Keep the snapshot intact. Changes
between collection and import leave the run incomplete.

## Plan and bundle formats

The plan is JSON with `version: 1` and an `invocations` object keyed by
invocation ID. Each entry has `variant`, `command` (an argv array),
`toolchain` (a string map with `rustc` and `cargo`, plus `clippy` or
`kani` as applicable), and a `checks` object.

| `checks.format` | Other fields |
| --- | --- |
| `clippy` | `package` is the Cargo JSON package ID, `target` the target name and `kind` its Cargo target kind. Supply one planned invocation for each required unit. |
| `kani_cbmc` | `harness`, positive `unwind`, arrays of `completeness` and `safety_checks` IDs, and an `obligations` object. Each obligation has `requirement`, `proposition`, `native_check`, its array of `reachability` check IDs and `assumptions`. |

The producer writes a single JSON file with `version: 1`, `producer` (a
CI run reference or producer identity), `inputs` (the snapshot object) and
`invocations` keyed by the same IDs. Each invocation supplies `command`,
`toolchain`, `exit_code`, `output` (UTF-8 native output as a string), and
`output_sha256` (hex SHA-256 of the UTF-8 bytes). Include the CBMC version
and solver identity in a proof entry's reviewed toolchain map.

Provide Cargo JSONL from a fresh Clippy run. A clean invocation needs an
artifact for the selected target and a successful terminal record. For proof
import, supply native CBMC JSON UI from a Kani-generated harness and the
CBMC exit status. The reviewed inventory must enumerate every emitted check.
Use a reachability assertion whose violation and trace witness that execution
reaches the obligations. Record every required unwinding check. An empty
completeness list is appropriate only where the reviewer establishes that the
harness and its callees have no obligations requiring it. Branch-specific
reachability and precondition adequacy still require review. The importer
preserves native output in the evidence inventory.

## Read results

```sh
safety rust import proof-bundle.json --path WORKSPACE
safety report WORKSPACE --run RUN-ID
```

An agent can call `safety.rust_import` with a workspace-relative `bundle`,
then page through proof properties with `safety.get_run`. Raw file references
point to the recorded native output. Proof outcomes apply to the declared
harness and model. Requirement approval stays outside the import. The
`examples/rust-interlock` crate provides the reset transition and two Kani
assertions for building a reference producer.

## Replay the interlock fixture

With the Kani 0.68.0 release's `kani-driver` and CBMC 6.11.0 on PATH, use the
matching nightly from the Kani release. For a manually unpacked release, make
the nightly's shared libraries available to its compiler on Linux.

```sh
cargo build --locked
python3 tests/integration/rust_proof_smoke.py
```

The script verifies both reset obligations, then removes the active-fault check
in a temporary copy and requires a witnessed violation of the original
obligation. It reuses the original reviewed check inventory for the second run.

## Cross-check a Kani model with ESBMC

An `esbmc_goto` check replays a Kani GOTO binary using ESBMC 8.7.0.
Supply `cbmc` (the paired invocation ID), workspace-relative `model`,
`model_sha256`, `entry` (the mangled harness), and the reviewed
`properties` from a complete replay. Record the binary in the input
snapshot and use the same variant, Kani version, Rust compiler and unwind
bound for both invocations. Include `toolchain.esbmc` in the replay.

```text
cbmc MODEL --function ENTRY --unwind N --unwinding-assertions --trace --json-ui
esbmc --binary MODEL --function ENTRY --ignore-property-class reachability_check --ignore-property-class cover --unwind N --multi-property
```

Supply the ESBMC result table and process exit status with both output
streams. Exact reviewed startup warnings about linked `signgam` and
`errno` declarations can be listed in `allowed_warnings`. The paired CBMC
record supplies reachability checks. ESBMC omits those classes on replay.
A divergence leaves the run incomplete and preserves each backend outcome.
An omitted result stays open even when ESBMC reports another witnessed
violation. Both checks share the Kani frontend and model.

## Import Verus results

A `verus` check uses pinned Verus `0.2026.11.02.8bf68ee` with the
Rust 1.99.1 toolchain. The reviewed entry specifies `source`, fully
qualified `function`, `verified_functions`, `requirement`, `proposition`
and optional `assumptions`. Use `toolchain.verus` for the Verus version.
Capture stdout as `output` and stderr as `diagnostics`.

```sh
verus --output-json --no-cheating --crate-type=lib proofs/interlock.rs
```

A failed verification condition leaves the obligation open. The reviewed
specification must describe the domain and its feasibility. The importer
records the proof under that specification and its library assumptions.

Replay the bounded and deductive interlock with the pinned tools on PATH.

```sh
cargo build --locked
python3 tests/integration/rust_proof_smoke.py --with-esbmc --with-verus
```

## Pilot an Aeneas and Lean refinement

The `aeneas_lean` import uses Hax 0.5.0, Charon nightly-2026.10.31,
Aeneas build-2b9e3ea, Hax Lean library v0.3.41 and Lean 4.35.0. Put these
versions in the `hax`, `charon`, `aeneas`, `hax-lean-lib` and `lean`
toolchain fields. Preserve the Rust source and extraction configuration,
generated Lean models, handwritten proof and probe in the input snapshot.

The reviewed entry supplies `source`, `proof` and `probe` paths, a
`models` map of generated Lean paths to SHA-256 digests, the import `module`,
`theorem`, exact one-line Lean `proposition`, `requirement`, and
`allowed_axioms`. The probe has these four lines and a final newline.

```lean
import Reviewed.Proof
set_option warningAsError true
#check (@reviewed_theorem : ReviewedProposition)
#print axioms reviewed_theorem
```

After fresh extraction and compilation, run `lake env lean --json PROBE`.
Supply the native JSON and the freshly regenerated digest map as
`regenerated_models`. The importer validates the probe and model digests
against the reviewed inputs and checks the theorem's transitive axiom list.
Proof holes and unreviewed axioms leave the obligation open. CI supplies
the extraction and build provenance.

The opt-in pilot uses several GiB for Lean dependencies.

```sh
cargo build --locked
python3 tests/integration/aeneas_proof_smoke.py --work-dir /path/to/lean-work
```

## Compare proof runs

```sh
safety rust compare --base RUN-ID --head RUN-ID --path WORKSPACE
```

The command integrity-checks both Rust runs and compares archived plans, the
proof IDs, claims, bounds and native scopes. Generated model digests may
change after an implementation change. Other plan changes prevent automatic
comparison. A formerly proven obligation that becomes open or violated
returns exit 1 with the regression. An incomplete comparison without a
confirmed regression returns exit 3. CI can use a reviewed baseline's run ID
as its protected input.

The same result is available from `safety.rust_compare` over MCP with
`base`, `head`, and optional `offset` and `limit`. Counts cover the full
comparison even when the response is paged.
