# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792788812044-23117-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1d60e1e2c2e0516a2819fe59f868042770fe739e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"e78cb83671f9e39b67d413247bf8fd9c6f881aae5b6927dd28d2fde771294d57","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- clang-tidy-19: unavailable or not allowlisted (required)
- clang-19: unavailable or not allowlisted (optional)
- llvm-cov-19: unavailable or not allowlisted (optional)
- llvm-profdata-19: unavailable or not allowlisted (optional)
- frama-c: unavailable or not allowlisted (required)
- esbmc: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.
