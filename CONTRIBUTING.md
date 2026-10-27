# Contributing

Use Rust 1.89 or later and LLVM 19.1.0 or a later 19.1.x release. Original contributions must be
Apache-2.0 compatible. Do not submit MISRA standards text. Review dependency
licenses before distributing binaries, containers or installers.

The Makefile is the reference for each step; run targets in this order:

```sh
make check            # version consistency, rustfmt and clippy
make test             # unit and default integration tests
make clang            # native module, indexer and ctest; see CMAKE_ARGS in the Makefile
make cache            # network: download and hash-verify the SARIF schema (once)
make validation       # check, clang, examples, real-Clang tests and validate.py
make full-validation  # validation plus real ESBMC (8.6.0 or later), Frama-C
                      # (25.0 or later) and CLI cancellation tests
make smoke            # end-to-end CLI and MCP run into build/e2e-smoke
```

Ordinary Cargo tests mark real-Clang tests ignored when not requested. They must
be explicitly executed before declaring Clang integration validated. Full
validation requires `clang-tidy-19`, `llvm-cov-19`, `llvm-profdata-19`, `clang-19`,
`safety-source-index`, `esbmc` and `frama-c` (with the WP plugin) on PATH; run
`safety doctor` to confirm their versions and capabilities. Never replace
real integration results with mocked output. The formal suites are opt-in; missing analyzers are skipped, never reported as passed.

Validation needs Python jsonschema 4.26.0 or later. JSON/SARIF checks use original committed schemas and an external SARIF schema cached during setup.
`make cache` downloads it and verifies the SHA-256 pinned in `docs/tool-versions.json`;
it is the only validation step that uses the network.

Example compilation databases are generated explicitly by
`python3 examples/configure.py`. The engine must never run that script itself.

Add positive, negative, boundary, macro, invalid-input and relevant dialect tests
with every custom check. Validate proposed MISRA mappings against a licensed
edition and record the reviewer-provided mapping scope and limitations. Do not
claim a guideline is covered merely because a similar Clang warning exists.

Keep adapters simple: describe argv and parse native results; use the common
runner. Every unsupported option or failed parse must remain incomplete.

For the mixed-language demonstration, put the built native indexer and external
LLVM/ESBMC tools on PATH, then run:

```sh
make smoke                       # throwaway output in build/e2e-smoke
python3 scripts/configure_demo.py && python3 scripts/e2e.py   # refresh committed evidence
```

The setup script explicitly builds and executes a known developer coverage test.
It is not called by the analysis engine. By default the end-to-end driver
preserves real outputs under `validation/completion/e2e` (`--output` selects
another directory) and expects scoped formal successes
and explicit incomplete MISRA/instrumentation scope. Also run
`python3 scripts/validate_harnesses.py` after `npm ci --prefix examples/agent-harnesses`
with Codex installed. See [installation](docs/installation.md) for the repeatable
offline reference container and packaging validation.
