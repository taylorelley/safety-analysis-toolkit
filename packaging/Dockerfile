# syntax=docker/dockerfile:1
# Runtime image: the toolkit, LLVM/Clang 19.1 and Frama-C (Eva and WP). On
# start it serves MCP over stdio for the mounted workspace. ESBMC is not
# bundled: the entrypoint fetches the pinned release on first start and
# verifies its SHA-256. See docs/installation.md ("Container").
ARG BASE=debian:bookworm-slim@sha256:7ff6ba31527b9e336162fa061746c366c4b2032f4d2cff2078bea43f78efe3f8

# apt.llvm.org, with the signing key fingerprint verified.
FROM ${BASE} AS llvm
RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates curl gnupg && \
    curl --proto '=https' --tlsv1.2 -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key -o /tmp/llvm.key && \
    gpg --show-keys --with-colons /tmp/llvm.key | grep -q '6084F3CF814B57C1CF12EFD515CF4D18AF4F7421' && \
    gpg --dearmor -o /usr/share/keyrings/llvm.gpg /tmp/llvm.key && \
    echo 'deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-19 main' \
      > /etc/apt/sources.list.d/llvm.list && \
    rm -rf /tmp/llvm.key /var/lib/apt/lists/*

FROM llvm AS builder
ENV PATH="/root/.cargo/bin:${PATH}"
RUN apt-get update && apt-get install -y --no-install-recommends g++ make cmake git python3 \
      clang-19 clang-tidy-19 llvm-19-dev libclang-19-dev libclang-rt-19-dev && \
    dpkg --compare-versions "$(clang-tidy-19 --version | sed -n 's/.*version \(19\.1\.[0-9]*\).*/\1/p')" ge 19.1.0 && \
    rm -rf /var/lib/apt/lists/* && \
    curl --proto '=https' --tlsv1.2 -fsSL https://sh.rustup.rs -o /tmp/rustup.sh && \
    sh /tmp/rustup.sh -y --profile minimal --default-toolchain stable && rm /tmp/rustup.sh
WORKDIR /src
COPY . .
RUN cargo build --locked --release && \
    cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build/clang --parallel "$(nproc)" && \
    ctest --test-dir build/clang --output-on-failure && \
    scripts/install.sh /opt/safety && \
    install -m 755 scripts/fetch_esbmc.sh /opt/safety/bin/safety-fetch-esbmc && \
    install -m 755 packaging/entrypoint.sh /opt/safety/bin/safety-entrypoint && \
    python3 -c "import json; d = json.load(open('docs/tool-versions.json')); \
print(f\"ESBMC_URL={d['esbmc_setup_download']}\nESBMC_SHA256={d['esbmc_download_sha256']}\")" \
      > /opt/safety/share/safety/esbmc.env

FROM llvm AS runtime
RUN apt-get update && apt-get install -y --no-install-recommends clang-19 clang-tidy-19 llvm-19 \
      frama-c-base git unzip tini && \
    rm -rf /var/lib/apt/lists/*
COPY --from=builder /opt/safety /opt/safety
# ESBMC is fetched here on first start; mount a named volume to keep it.
RUN install -d -m 1777 /opt/esbmc /workspace
ENV SAFETY_ESBMC_DIR=/opt/esbmc \
    PATH="/opt/safety/bin:/opt/esbmc/bin:${PATH}"
# Fail the build unless every bundled capability works.
RUN safety --version && safety-source-index --version && \
    clang-tidy-19 --load=/opt/safety/lib/safety/SafetyMisra.so '--checks=-*,misra-c2012-*' --list-checks | grep -q misra-c2012- && \
    frama-c -plugins | awk '{print $1}' | grep -qx WP && \
    frama-c -plugins | awk '{print $1}' | grep -qx Eva
USER 1000:1000
WORKDIR /workspace
ENTRYPOINT ["/usr/bin/tini", "--", "/opt/safety/bin/safety-entrypoint"]
CMD ["mcp"]
