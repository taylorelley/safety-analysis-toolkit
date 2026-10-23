# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791466384-71976-0

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Original project-policy checks enabled; MISRA mappings unvalidated
- No structured diagnostics export for /workspace/safety/build/demo/src/controller.cpp
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete
