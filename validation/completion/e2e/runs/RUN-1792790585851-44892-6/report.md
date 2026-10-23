# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790585851-44892-6

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.
