# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782806056-19454-0

Status: Incomplete; complete: false

Translation units: 5 total, 2 analyzed, 0 failed, 0 unsupported, 3 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Formal properties


## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.
