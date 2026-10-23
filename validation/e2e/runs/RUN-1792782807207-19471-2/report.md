# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782807207-19471-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Formal properties

- main.assertion.1: Proven; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.
