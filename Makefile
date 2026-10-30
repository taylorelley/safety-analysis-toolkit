.PHONY: build check test clang validation full-validation examples smoke cache image container-smoke
build:
	cargo build --locked --release
check:
	python3 scripts/check_versions.py
	cargo fmt --check
	cargo clippy --locked --all-targets -- -D warnings
test:
	cargo test --locked
# Pass a non-default LLVM with, e.g.,
# make clang CMAKE_ARGS="-DLLVM_DIR=/path/lib/cmake/llvm -DClang_DIR=/path/lib/cmake/clang"
CMAKE_ARGS ?=
clang:
	cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release $(CMAKE_ARGS)
	cmake --build build/clang --parallel 2
	ctest --test-dir build/clang --output-on-failure
examples:
	python3 examples/configure.py
# Network: downloads the external SARIF schema and verifies its pinned hash.
cache:
	python3 scripts/cache_sarif.py
validation: check clang examples
	PATH="$(CURDIR)/build/clang:$$PATH" SAFETY_TEST_MODULE="$(CURDIR)/build/clang/SafetyMisra.so" cargo test --locked -- --include-ignored --skip real_esbmc_properties --skip real_frama_c --skip real_esbmc_induction_harness
	cargo build --locked --release
	python3 scripts/validate.py

# End-to-end CLI and MCP smoke test into a throwaway directory. Requires all
# analyzers on PATH; configure_demo.py builds and runs the demo coverage test.
smoke: build
	# Start from an empty demo so e2e.py copies only this invocation's runs.
	rm -rf build/e2e-smoke build/demo
	python3 scripts/configure_demo.py
	python3 scripts/e2e.py --output build/e2e-smoke

# Runtime image serving MCP over stdio; ESBMC is fetched on first start.
IMAGE ?= safety-toolkit:local
image:
	docker build -f packaging/Dockerfile -t $(IMAGE) .
container-smoke:
	python3 scripts/container_smoke.py $(IMAGE)

full-validation: validation
	esbmc --version
	cargo test --locked real_esbmc -- --ignored
	frama-c -version
	cargo test --locked real_frama_c -- --ignored
	python3 tests/integration/cli_cancel.py target/release/safety
