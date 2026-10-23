# Contributing

Use the pinned Rust toolchain and LLVM 19.1.7. Original contributions must be
Apache-2.0 compatible. Do not submit MISRA standards text. Review dependency
licenses before distributing binaries, containers or installers.

```sh
cargo build --locked
cargo fmt --check
cargo clippy --locked --all-targets -- -D warnings
cargo test --locked
cmake -S clang -B build/clang -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm \
  -DClang_DIR=/path/to/llvm/lib/cmake/clang
cmake --build build/clang --parallel 2
ctest --test-dir build/clang --output-on-failure
PATH="$PWD/build/clang:$PATH" SAFETY_TEST_MODULE="$PWD/build/clang/SafetyMisra.so" \
  cargo test --locked -- --include-ignored --skip real_esbmc_properties
# With externally installed ESBMC 8.6.0:
cargo test --locked real_esbmc_properties -- --ignored
```

Ordinary Cargo tests mark real-Clang tests ignored when not requested. They must
be explicitly executed before declaring Clang integration validated. Full
validation requires `clang-tidy-19`, `llvm-cov-19`, `llvm-profdata-19`, `clang-19`, `safety-source-index` and `esbmc` on PATH and all checks above. Never replace
real integration results with mocked output. The real ESBMC suite is opt-in. Frama-C execution tests remain unavailable and its phase is incomplete.

Use `make full-validation` with installed dependencies and Python jsonschema
4.26.0. JSON/SARIF checks use original committed schemas and an external SARIF schema cached during setup:

```sh
mkdir -p .cache
curl -fsSL https://json.schemastore.org/sarif-2.1.0.json -o .cache/sarif-2.1.0.json
echo "c96eb2d311c37b0a38cbd18c52d79a68f778bbd2d831abed7412d9850740f785  .cache/sarif-2.1.0.json" | sha256sum -c -
```

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
python3 scripts/configure_demo.py
python3 scripts/e2e.py
```

The setup script explicitly builds and executes a known developer coverage test.
It is not called by the analysis engine. The end-to-end driver preserves real
outputs under `validation/e2e` and expects incomplete results for unsupported
Frama-C, unvalidated MISRA mappings and uncovered/incomplete source scope.
