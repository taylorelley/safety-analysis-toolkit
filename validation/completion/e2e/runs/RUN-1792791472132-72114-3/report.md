# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791472132-72114-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-40d3208700e67686b11f: PROVEN; /workspace/safety/build/demo/src/contract.c:3 identity postcondition: \result ≡ \old(x) [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), wp, entry identity, x86 64-bit model; supplied ACSL contracts; WP typed memory model, RTE obligations and Qed only. Unwind bound: None; model completeness established: true. Supplied assumptions: []. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.
- FC-7ff603969eb560de72c3: PROVEN; /workspace/safety/build/demo/src/contract.c:4 identity assigns clause: assigns \nothing; [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), wp, entry identity, x86 64-bit model; supplied ACSL contracts; WP typed memory model, RTE obligations and Qed only. Unwind bound: None; model completeness established: true. Supplied assumptions: []. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.
