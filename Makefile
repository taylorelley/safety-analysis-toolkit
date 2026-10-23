.PHONY: build check test clang validation full-validation examples
build:
	cargo build --locked --release
check:
	cargo fmt --check
	cargo clippy --locked --all-targets -- -D warnings
test:
	cargo test --locked
clang:
	cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release
	cmake --build build/clang --parallel 2
	ctest --test-dir build/clang --output-on-failure
examples:
	python3 examples/configure.py
validation: check clang examples
	PATH="$(CURDIR)/build/clang:$$PATH" SAFETY_TEST_MODULE="$(CURDIR)/build/clang/SafetyMisra.so" cargo test --locked -- --include-ignored --skip real_esbmc_properties --skip real_frama_c --skip real_esbmc_induction_harness
	cargo build --locked --release
	python3 scripts/validate.py

full-validation: validation
	esbmc --version
	cargo test --locked real_esbmc -- --ignored
	frama-c -version
	cargo test --locked real_frama_c -- --ignored
	python3 tests/integration/cli_cancel.py target/release/safety
