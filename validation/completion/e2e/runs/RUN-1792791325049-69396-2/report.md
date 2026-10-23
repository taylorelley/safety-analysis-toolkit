# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791325049-69396-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791325049-69396-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791325049-69396-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791325049-69396-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.
