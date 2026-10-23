# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783104485-21007-0

Status: Incomplete; complete: false

Translation units: 1 total, 0 analyzed, 0 failed, 1 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: .

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- eva-analysis: NOT_CHECKED; Frama-C Eva verification not checked. Scope: No validated Frama-C installation/output adapter is available in this release. Unwind bound: None; model completeness established: false. Supplied assumptions: []. Evidence: .

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Frama-C Eva/WP execution is unavailable pending a pinned, real integration test. No proof is inferred from installation or empty output.
