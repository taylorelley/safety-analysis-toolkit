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
cargo test --locked -- --include-ignored
```

Ordinary Cargo tests mark real-Clang tests ignored when not requested. They must
be explicitly executed before declaring Clang integration validated. Full
validation requires `clang-tidy-19` on PATH and all checks above. Never replace
real integration results with mocked output. Tests that require unavailable
formal analyzers do not exist yet; those phases remain unimplemented.

Use `make full-validation` with installed dependencies and Python jsonschema
4.26.0. JSON/SARIF schema checks use the committed schema, cached during setup.
Example compilation databases are generated explicitly by
`python3 examples/configure.py`. The engine must never run that script itself.

Add positive, negative, boundary, macro, invalid-input and relevant dialect tests
with every custom check. Validate proposed MISRA mappings against a licensed
edition and record the reviewer-provided mapping scope and limitations. Do not
claim a guideline is covered merely because a similar Clang warning exists.

Keep adapters simple: describe argv and parse native results; use the common
runner. Every unsupported option or failed parse must remain incomplete.
