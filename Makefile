.PHONY: build check test clang full-validation examples
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
full-validation: check clang examples
	cargo test --locked -- --include-ignored
	cargo build --locked --release
	python3 scripts/validate.py
