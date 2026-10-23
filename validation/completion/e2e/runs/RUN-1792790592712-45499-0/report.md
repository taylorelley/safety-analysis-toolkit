# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790592712-45499-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Doctor establishes discovery only; it does not analyze source.
