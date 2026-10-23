# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791480107-72416-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: ["User-supplied harness assumption (not approved by this tool): enabled is fixed to one by this harness; other inputs are outside its scope"]. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.
