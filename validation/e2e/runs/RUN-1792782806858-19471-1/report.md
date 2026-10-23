# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782806858-19471-1

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Formal properties


## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete
