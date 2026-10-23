# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782809598-19471-4

Status: Findings; complete: true

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant manifest)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant manifest)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant manifest)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant manifest)

## Formal properties


## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Call graph extraction and complete template-specialization coverage are outstanding.
