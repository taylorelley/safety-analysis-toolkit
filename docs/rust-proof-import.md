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
