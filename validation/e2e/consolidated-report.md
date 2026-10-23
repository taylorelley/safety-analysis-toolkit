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

