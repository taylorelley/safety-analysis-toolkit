# Installation and repeatable validation

The supported reference is Linux x86-64, Rust 1.99.1, LLVM/Clang 19.1.7,
Frama-C Manganese 25.0-beta and ESBMC 8.6.0 with Z3. Use the exact native
LLVM version for both the extension and clang-tidy host. Other compiler flags,
versions, concurrency models and targets can be unsupported; discovery is not
a claim of analyzer compatibility.

Install CMake >=3.25, a C++17 compiler, Git, Python 3/jsonschema and the external
analyzers. The Debian bookworm reference package for Frama-C is
`20220511-manganese-3+b4`. The reference Dockerfile verifies the LLVM signing key
and installed LLVM version, pins the base image and Rust release, and caches
locked Cargo sources and the content-pinned SARIF schema. OS security packages
are resolved during setup; the resulting image digest and installed versions,+rather than the base digest alone, identify an executed reference environment.

```sh
cargo build --locked --release
cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_DIR=/usr/lib/llvm-19/lib/cmake/llvm \
  -DClang_DIR=/usr/lib/llvm-19/lib/cmake/clang
cmake --build build/clang --parallel 2
scripts/install.sh /absolute/install/prefix
export PATH="/absolute/install/prefix/bin:$PATH"
```

`install.sh` also creates a locked Rust license inventory, installs the original
Rust executable, native indexer/module, profiles, schemas, skill and notices.
It uses CMake installation to remove development build RPATHs. The installed
indexer still requires external LLVM shared libraries. Configure the trusted
module path explicitly; never load a plugin from an untrusted analyzed project.

ESBMC is external. The tested upstream download and SHA-256 are recorded in
`docs/tool-versions.json`. Review its solver/component licenses and choose an
appropriate open-source build. Neither installation nor release packaging
downloads, bundles or approves that upstream binary for redistribution.

Allow at least 15 GiB free for a fresh image/build; Docker storage drivers may
require additional space for filesystem-layer copies.

## Full native check

After explicit dependency setup and SARIF-schema caching as described in
CONTRIBUTING.md:

```sh
export PATH="$PWD/build/clang:$PATH"
make full-validation
python3 scripts/configure_demo.py
python3 scripts/e2e.py
python3 scripts/benchmark.py
npm ci --prefix examples/agent-harnesses
python3 scripts/validate_harnesses.py
```

The demo setup deliberately compiles and executes a developer coverage fixture;
running this script authorizes that test execution. The analysis executable
never runs the setup script, a project build or project tests. MCP host checks
require installed Codex/Pi and exercise their actual tool transports without
cloud model inference. Normal toolkit usage requires neither host nor Node.

For a clean reference installation, with Docker and an externally reviewed
ESBMC bin directory available:

```sh
SAFETY_ESBMC_DIR=/absolute/esbmc/bin scripts/reference_validation.sh \
  > validation/completion/reference-validation.log 2>&1
```

Image/dependency setup uses the network and verified TLS; a system CA is supplied
as a transient build secret. The subsequent fresh source copy, format/lint/tests,
real native/formal/coverage integrations, installation, benchmark and package
reproducibility checks run with `--network none` and Cargo offline. The original
source mount and external ESBMC mount are read-only. Results go into
`build/reference-results`. The image is local validation infrastructure and is
not a reviewed redistributable analyzer bundle.

## Package original artifacts

```sh
python3 scripts/package.py
sha256sum -c build/safety-0.1.0-linux-x86_64.tar.gz.sha256
```

The archive normalizes tar ownership/timestamps and gzip metadata, retains
dependency license/notice texts, and includes only original toolkit binaries,
documentation and data files. Repeat packaging of the same built artifacts is
checked for byte equality. This does not claim compiler-level reproducibility
across arbitrary machines or toolchain installations.

After setup, `cargo build --locked --offline` and every analysis operation work
without Internet access. Missing optional analyzers are visible capability gaps;
an enabled missing/incompatible analyzer produces an incomplete operation.
