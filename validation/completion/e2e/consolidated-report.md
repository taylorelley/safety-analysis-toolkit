# Consolidated safety evidence

Each operation retains its own scope and build variants. This is draft evidence, not a compliance or certification decision.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782664571-18081-0

Status: Incomplete; complete: false

Translation units: 4 total, 4 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 11. Git metadata: `{"commit":"51cf37210e5ee7c698613c1de8a19de7ad2c36f3","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782665270-18104-0

Status: Incomplete; complete: false

Translation units: 4 total, 4 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/navigation.c:6:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 11. Git metadata: `{"commit":"51cf37210e5ee7c698613c1de8a19de7ad2c36f3","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782802511-19347-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782803158-19370-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782803459-19395-0

Status: Incomplete; complete: false

Translation units: 1 total, 0 analyzed, 0 failed, 1 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: .

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties

- eva-analysis: NOT_CHECKED; Frama-C Eva verification not checked. Scope: No validated Frama-C installation/output adapter is available in this release. Unwind bound: None; model completeness established: false. Supplied assumptions: []. Evidence: .

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Frama-C Eva/WP execution is unavailable pending a pinned, real integration test. No proof is inferred from installation or empty output.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782803513-19402-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782804659-19417-0

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782805832-19432-0

Status: Findings; complete: true

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant manifest)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant manifest)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant manifest)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant manifest)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `9019f16f0ae651d92b307985ffdb8c74e8e477c69313973c43559b09f8ade806`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Call graph extraction and complete template-specialization coverage are outstanding.

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

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782806187-19471-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782806858-19471-1

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782807207-19471-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782808371-19471-3

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782809598-19471-4

Status: Findings; complete: true

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant manifest)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant manifest)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant manifest)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant manifest)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `9019f16f0ae651d92b307985ffdb8c74e8e477c69313973c43559b09f8ade806`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Call graph extraction and complete template-specialization coverage are outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792782809820-19471-5

Status: Incomplete; complete: false

Translation units: 5 total, 2 analyzed, 0 failed, 0 unsupported, 3 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Configuration snapshot is unavailable in this legacy record.
Input files fingerprinted: 13. Git metadata: `{"commit":"d5f1a75d04087a334eb70f9614f60e10d41ae79e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `null`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783103500-20959-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
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
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783104169-20982-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

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
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783104544-21014-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783105686-21029-0

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783106874-21044-0

Status: Findings; complete: true

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant manifest)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant manifest)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant manifest)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant manifest)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `9019f16f0ae651d92b307985ffdb8c74e8e477c69313973c43559b09f8ade806`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Call graph extraction and complete template-specialization coverage are outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783107099-21066-0

Status: Incomplete; complete: false

Translation units: 5 total, 2 analyzed, 0 failed, 0 unsupported, 3 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783107232-21083-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
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
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783107903-21083-1

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

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
- Custom goto project-policy check enabled; MISRA mapping unvalidated
- Configured MISRA edition has no validated enforcement mapping; guideline verification remains incomplete

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783108254-21083-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783109389-21083-3

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. No induction-based proof is claimed.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783110555-21083-4

Status: Findings; complete: true

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant manifest)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant manifest)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant manifest)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant manifest)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `9019f16f0ae651d92b307985ffdb8c74e8e477c69313973c43559b09f8ade806`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Call graph extraction and complete template-specialization coverage are outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792783110780-21083-5

Status: Incomplete; complete: false

Translation units: 5 total, 2 analyzed, 0 failed, 0 unsupported, 3 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1c383de6faa9a147d1cc929050a52338169c9d37","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"5ab38708298fce5cc4a25b259f34478b9f561c83158c1e91696eb50bffb19efe","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792789267755-25187-0

Status: Incomplete; complete: false

Translation units: 5 total, 5 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023"}`.

Input files fingerprinted: 13. Git metadata: `{"commit":"1d60e1e2c2e0516a2819fe59f868042770fe739e","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"72352625830241b41690cc066c16122b0154ca6a23412d60add2c0b4afad6ea5","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang-tidy`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `e4f4dd201e1e44451278c5302c687cffc7b1bf7c17102e7c58cfcdba88f5e69a`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: Debian clang version 19.1.7 (3+b1) Target: x86_64-pc-linux-gnu Thread model: posix InstalledDir: /workspace/scratch/llvm/usr/lib/llvm-19/bin; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-profdata`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `37768e7a46a090f7af39a1490d14f6f1c7b2567c31030069b3caa74d5c429531`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
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
- frama-c: unavailable or not allowlisted (required)
- Doctor establishes discovery only; it does not analyze source.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790576776-44728-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790577477-44754-0

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790577863-44782-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790577863-44782-0/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790577863-44782-0/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790577863-44782-0/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790578194-44801-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790578610-44820-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790579752-44835-0

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790580928-44850-0

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790581187-44875-0

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790581314-44892-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790582033-44892-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790582455-44892-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790582455-44892-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790582455-44892-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790582455-44892-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790582898-44892-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790583340-44892-4

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790584698-44892-5

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790585851-44892-6

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790586146-44892-7

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790586814-45183-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790587608-45183-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790588076-45183-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790588076-45183-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790588076-45183-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790588076-45183-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790588536-45183-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790589806-45183-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790591092-45183-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790591365-45183-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790593434-45499-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790593859-45499-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790593859-45499-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790593859-45499-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790593859-45499-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790594206-45499-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790595475-45499-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790596708-45499-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790597006-45499-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790894523-55149-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790895323-55149-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790895776-55149-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790895776-55149-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790895776-55149-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790895776-55149-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790896217-55149-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790897462-55149-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790898757-55149-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790899029-55149-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790900437-55366-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790901201-55366-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790901648-55366-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790901648-55366-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790901648-55366-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792790901648-55366-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790902004-55366-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790903258-55366-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790904524-55366-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792790904803-55366-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"eacb5f92a21800e931fd5d14147597a5a084fc54","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"6f51485dc8ef87a2baaffc06c830a2ddc349d07770405a02afe9ce0527e99837","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791299080-67708-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791300382-67738-0

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791301296-67770-0

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

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791301296-67770-0/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791301296-67770-0/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791301296-67770-0/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791302004-67792-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791302684-67814-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791306477-67977-0

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791309277-68100-0

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791310076-68229-0

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791310491-68379-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791312211-68379-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791312899-68379-2

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

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791312899-68379-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791312899-68379-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791312899-68379-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791313368-68379-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791314191-68379-4

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791315784-68379-5

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791316971-68379-6

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791317243-68379-7

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791318419-68970-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791319246-68970-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791319661-68970-2

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

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791319661-68970-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791319661-68970-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791319661-68970-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791320013-68970-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791321197-68970-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791322380-68970-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791322645-68970-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791323903-69396-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791324630-69396-1

Status: Incomplete; complete: false

Translation units: 6 total, 5 analyzed, 1 failed, 0 unsupported, 0 excluded.

## Findings

- harness/../src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/navigation.c:5:16 [safety-prohibited-goto] goto statement requires project policy review (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)
- src/navigation.c:6:10 [clang-analyzer-core.NullDereference] Dereference of null pointer (loaded from variable 'p') (variant VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e)

## Selected profiles and scope

Operation: scan. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791325383-69396-3

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791326562-69396-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791327802-69396-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791328096-69396-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"d664e9a2a1956117859918e70372eb7ab55087e9","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"d46be987f349bce722fb731f49bef56c1e9b5795305f51a01b0b1e3c8e956379","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791465619-71950-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791466822-72004-0

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791466822-72004-0/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791466822-72004-0/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791466822-72004-0/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791467167-72023-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791467683-72042-0

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791468951-72057-0

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791470225-72072-0

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791470505-72097-0

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791470643-72114-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791471375-72114-1

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791471791-72114-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791471791-72114-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791471791-72114-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791471791-72114-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791472557-72114-4

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

- main.assertion.1: PROVEN; line 5  assertion navigate(1) == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: true. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791473735-72114-5

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791474948-72114-6

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791475249-72114-7

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791477179-72416-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791478479-72416-1

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791479455-72416-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791479455-72416-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791479455-72416-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791479455-72416-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791482163-72416-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791484360-72416-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791484965-72416-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791487953-72934-0

Status: Passed; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: doctor. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791489206-72934-1

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791490270-72934-2

Status: Passed; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/bin/frama-c`: 25.0-beta (Manganese); SHA-256 `11104c8b2a159a065f689b3bc0406b0f992ba0eb7323eaadf2dc4a1f0a628bc6`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/clang`: version not separately probed; SHA-256 `b8cbb52c887ed115396131d97cd0e7a7a64dfb98d0c8d37c135f8853a6999682`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- FC-6437020dc46f84c8b657: PROVEN; /workspace/safety/build/demo/harness/navigation_harness.c:5 main precondition of __FC_assert: nonnull_c: c ≢ 0 [native status: Valid]. Scope: Frama-C 25.0-beta (Debian Manganese), eva, entry main, x86 64-bit model; reachable sequential C states; alarms and native assumptions apply. Unwind bound: None; model completeness established: true. Supplied assumptions: ["Native model premise (Valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791490270-72934-2/FRAMAC_SHARE/libc/assert.h:31 __FC_assert precondition: nonnull_c: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791490270-72934-2/FRAMAC_SHARE/libc/assert.h:32 __FC_assert termination clause: c ≢ 0", "Native model premise (Considered valid): /workspace/safety/build/demo/.safety/runs/RUN-1792791490270-72934-2/FRAMAC_SHARE/libc/assert.h:35 __FC_assert assigns clause: assigns \\nothing;"]. Evidence: raw/frama-c.log, raw/frama-c-properties.tsv.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Results are scoped to the selected entry, C data model, supplied contracts and preserved native assumptions. Sequential models only; concurrency and unmodeled environment behavior are not verified. No human approval of assumptions is inferred.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791491109-72934-3

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

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791493172-72934-4

Status: Findings; complete: true

Translation units: 1 total, 1 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings


## Selected profiles and scope

Operation: prove. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/esbmc/release/bin/esbmc`: ESBMC version 8.6.0 64-bit x86_64 linux; SHA-256 `4f9e846eaa56be8ba0f6b50c5c2772b1e488e82ab35e4e71e79627b6eed2ce2b`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties

- main.assertion.1: VIOLATED; line 7  assertion x == 1. Scope: ESBMC 8.6.0 sequential source model with entry main; 64-bit little-endian; Z3; finite unwind 8; default unwinding assertions enabled; source assumptions and library models apply. Unwind bound: Some(8); model completeness established: false. Supplied assumptions: []. Evidence: raw/esbmc.log.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Advisory source-model verification only. No independently approved harness/environment assumptions were supplied. Counterexamples are model traces, not automatically confirmed production defects. Proof method and induction search limits, where selected, are retained in verification.json and native phase logs.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791495186-72934-5

Status: Findings; complete: true

Translation units: 6 total, 6 analyzed, 0 failed, 0 unsupported, 0 excluded.

## Findings

- harness/navigation_harness.c:5:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f)
- src/controller.cpp:3:21 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/controller.cpp:4:5 [trace-46b45d335c4ec8bb] Untraced code symbol controller::apply (variant VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e)
- src/test.c:4:5 [trace-8364526acdaa3f89] Annotations differ across translation-unit contexts; automatic mapping is ambiguous (variant VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180)
- trace.yaml:1:1 [trace-2c678235a40616be] Requirement R-ORPHAN has no linked implementation (variant default)
- trace.yaml:1:1 [trace-370b43f8611d36ef] Broken reference R-ORPHAN -> UNKNOWN-TEST (variant default)
- trace.yaml:1:1 [trace-d57bd5409c13e002] Requirement R-CTRL has no linked test (variant default)
- trace.yaml:1:1 [trace-db4027b75e18935b] Requirement R-ORPHAN has no linked test (variant default)

## Selected profiles and scope

Operation: trace. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/safety/build/clang/safety-source-index`: version not separately probed; SHA-256 `fe9f7a8389eda679ebc3365016d895096723e229c6d8c7f3a8eca90e60b80399`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Traceability

Relationship gaps are listed above. Indexed symbols and the explicit graph are preserved in `trace.json`; links do not establish semantic correctness.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Trace links establish recorded relationships only, not semantic requirements satisfaction. Direct call edges and template identities are retained; indirect calls/lambda ownership are explicitly unresolved and require review.

# Safety analysis evidence

Project: mixed-safety-demo

Run: RUN-1792791495679-72934-6

Status: Incomplete; complete: false

Translation units: 6 total, 2 analyzed, 0 failed, 0 unsupported, 4 excluded.

## Findings

- src/decision.c:1:1 [coverage-gap-branches] branches: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:1:1 [coverage-gap-mcdc] mcdc: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/decision.c:2:32 [coverage-mcdc-condition-2] Condition 2 has no measured MC/DC independence pair; investigate requirements-based tests, infeasibility and instrumentation scope (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-branches] branches: 1/2 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)
- src/test.c:1:1 [coverage-gap-regions] regions: 3/4 measured items covered; investigate linked requirements and requirements-based tests (variant coverage-artifact)

## Selected profiles and scope

Operation: coverage. Assurance: `null`. Coding editions: `{"c":"MISRA-C:2012","cpp":"MISRA-CPP:2023","deviations":null,"recategorization":null,"registries":[]}`.

Input files fingerprinted: 14. Git metadata: `{"commit":"7e2405cf9326bd7d109adfdcb71423f9c19fa8f4","dirty":true}`. Build variants: VAR-59d26e56ac42efbf896ef09f1788fdd1129c016477634b0d86e449897570a99e, VAR-14293ae00cdb67bf01d89dd6357f091a59dc38a7daa885428bd03167fd741f0e, VAR-6c61e71e5d659d81fd75c68472b6a04b5d165baafee2350d9ddb485cb3c8c31b, VAR-9ff35ff903434396903470597bbfe339f5cb6dcdd3927ada3c925391aaf00180, VAR-03fed98ba753967d5cf07a4ee49a2b95acaf802cb4c8e6c507e5ea69ea3d3d43, VAR-894f317f36556005a1e7ba2065d4ad58dec7ce4de0eba4c541f626e085e5861f.

## Tools and provenance

Toolkit identity: `{"executable_sha256":"74491fdf99771f19e3f392cdd9e8416984d9062d877f00e288fe2996a3d6ddae","host_arch":"x86_64","version":"0.1.0"}`.

- `/usr/local/bin/git`: version not separately probed; SHA-256 `9dae295c1a4ffae4ee772e6205fcf22b26e1bfeeb5ee98f278b5139bda7ca22f`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.
- `/workspace/scratch/llvm/usr/lib/llvm-19/bin/llvm-cov`: Debian LLVM version 19.1.7   Optimized build.; SHA-256 `8421a134d7c6bc9ca8dc1fb5b9ef40adc6457b78f5066b6202b2072256f83866`. Restrictions: network denied; filesystem isolation unavailable; trusted read-only analyzer invocation.

## MISRA draft status

No guideline mapping is validated. GEP/GCS metadata in `misra-drafts.json` is incomplete draft evidence; no guideline is marked compliant from analyzer silence.

## Formal properties


## Structural coverage

Measured gaps are listed above. Native regions, branches and MC/DC records are preserved in `coverage.json` and `raw/llvm-cov.json`. Missing provenance or excluded units leave scope incomplete.

## Evidence references

This run's `manifest.json`, `config.json`, `findings.json` and SHA-256 inventory `evidence.json` bind the inputs, exact argv/environment and native files. Evidence hashes are verified when loading or exporting; they are not an authenticity signature.

## Outstanding work and limitations

- Automated evidence for human review; no certification or compliance approval.
- MISRA coverage, formal verification, traceability and structural coverage are not established by static analysis.
- System headers/toolchain dependencies are not yet fingerprinted; replay requires the same reference environment.
- Decisions exist without measured MC/DC conditions; MC/DC instrumentation completeness is unknown
- Coverage import never executes tests. Execution environment and requirement/test links in provenance are supplied records, not independently validated claims. LLVM branch coverage is not automatically DO-178C decision coverage. Data/control coupling, object-code mapping, target execution assurance and qualification assessment remain outstanding.

