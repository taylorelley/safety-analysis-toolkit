# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782803459-19395-0

Status: Incomplete; complete: false

Translation units: 1 total, 0 analyzed, 0 failed, 1 unsupported, 0 excluded.

## Findings


## Formal properties

- eva-analysis: NotChecked; Frama-C Eva verification not checked. Scope: No validated Frama-C installation/output adapter is available in this release

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Frama-C Eva/WP execution is unavailable pending a pinned, real integration test. No proof is inferred from installation or empty output.
