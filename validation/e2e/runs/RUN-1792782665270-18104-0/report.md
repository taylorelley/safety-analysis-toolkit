# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782665270-18104-0

Status: Incomplete; complete: false

Translation units: 4 total, 4 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/navigation.c:6:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Formal properties


## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete
