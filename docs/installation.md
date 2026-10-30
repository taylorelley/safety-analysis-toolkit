# Installation and repeatable validation

Minimum versions are Rust 1.89, LLVM/Clang 19.1.0 (later 19.1.x patch releases
accepted), Frama-C 25.0 (Manganese) and ESBMC 8.6.0 with Z3. The validated
reference is Linux x86-64 with Rust 1.99.1, LLVM/Clang 19.1.7, Frama-C
Manganese 25.0-beta and ESBMC 8.6.0; other supported releases are accepted, each
run records that their output formats have not been revalidated, and parsers fail closed on
unrecognised output. Build the extension and run clang-tidy with the same
LLVM 19.1.x release. Other compiler flags,
versions, concurrency models and targets can be unsupported; discovery is not
a claim of analyzer compatibility.

## Quick install

`scripts/bootstrap.sh` installs every dependency, builds the toolkit from source,
runs its tests, installs it under a prefix (default `~/.local`) and finishes with
a `safety doctor` self-check that every capability works. It supports Debian 12
and Ubuntu 22.04/24.04, skips anything already suitable, and uses `sudo` only
for missing apt packages.

```sh
scripts/bootstrap.sh                      # or --prefix /opt/safety
export PATH="$HOME/.local/bin:$PATH"      # ahead of system directories
```

- LLVM/Clang 19.1: the distribution's packages when they are 19.1.x, otherwise
  apt.llvm.org after verifying its signing key fingerprint.
- Rust: rustup, when no toolchain at the minimum version is installed.
- Frama-C with Eva and WP: Debian's `frama-c-base` includes WP; Ubuntu's does
  not, so on Ubuntu the pinned opam package (`frama_c_opam_package` in
  `docs/tool-versions.json`) is built in an opam switch, which takes about 20
  minutes. An existing switch with a suitable Frama-C is reused.
  `--frama-c apt|opam|skip` overrides the choice.
- ESBMC: the release pinned in `docs/tool-versions.json`, SHA-256 verified, under
  `PREFIX/opt`. Use `--esbmc-zip FILE` for a pre-downloaded archive (for example
  on a restricted network) or `--skip-esbmc`.

## Manual install

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
`docs/tool-versions.json`. `scripts/bootstrap.sh` and the runtime container
download that release onto the machine that runs it and verify its hash; no
image, archive or release package bundles or redistributes it. Review its
solver/component licenses before use.

## Container

`packaging/Dockerfile` builds a runtime image with the toolkit, LLVM/Clang 19.1
and Debian's Frama-C (Eva and WP). The build runs the native test suites and
fails unless the MISRA module loads and Frama-C provides WP and Eva. On start
the container serves MCP over stdio for its working directory:

1. ESBMC is fetched from the pinned URL into `/opt/esbmc` and its SHA-256 is
   verified (first start only when `/opt/esbmc` is a named volume). Without
   network access, or with `SAFETY_ESBMC=skip`, the server still starts and
   `doctor` reports ESBMC as unavailable. `SAFETY_ESBMC_ZIP` points at a
   pre-downloaded archive instead.
2. If the workspace has no `safety.yaml`, one is created for the container's
   tool paths with the MISRA module, Frama-C and ESBMC enabled
   (`SAFETY_AUTO_INIT=0` disables this). Review it before relying on results. A
   `safety.yaml` written on the host has host paths in its allowlist; let the
   container create its own.
3. `safety mcp` starts. stdout carries only MCP; container messages go to stderr.

```sh
make image                    # docker build -f packaging/Dockerfile -t safety-toolkit:local .
make container-smoke          # MCP end-to-end check of the built image
```

Mount the project at the same absolute path it has on the host, because
`compile_commands.json` records absolute paths, and run as your own user so
evidence is written with your ownership:

```sh
docker run -i --rm --user "$(id -u):$(id -g)" \
  -v safety-esbmc:/opt/esbmc -v "$PWD:$PWD" -w "$PWD" safety-toolkit:local
```

Once ESBMC is in the volume, add `--network none`: analysis needs no network.
Any other arguments run that `safety` command instead of the server, for
example `... safety-toolkit:local doctor "$PWD"`. MCP clients use the same
`docker run -i ...` command as their server command (see [MCP](mcp.md)).
The image contains LLVM and Frama-C (LGPL-2.1) but not ESBMC; review
[dependencies](dependencies.md) before distributing it.

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
