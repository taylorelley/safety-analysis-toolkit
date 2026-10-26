# syntax=docker/dockerfile:1
FROM debian:bookworm-slim@sha256:7ff6ba31527b9e336162fa061746c366c4b2032f4d2cff2078bea43f78efe3f8
ENV PATH="/root/.cargo/bin:/usr/lib/llvm-19/bin:${PATH}"
WORKDIR /opt/safety-cache
COPY Cargo.toml Cargo.lock ./
COPY src ./src
COPY profiles ./profiles
RUN --mount=type=secret,id=system_ca,required=true \
    apt-get update && apt-get install -y --no-install-recommends ca-certificates curl gnupg g++ make cmake git python3 python3-jsonschema frama-c-base=20220511-manganese-3+b4 xz-utils && rm -rf /var/lib/apt/lists/* && \
    \
    curl --cacert /run/secrets/system_ca -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key -o /tmp/llvm.key && \
    gpg --show-keys --with-colons /tmp/llvm.key | grep '6084F3CF814B57C1CF12EFD515CF4D18AF4F7421' && \
    gpg --dearmor -o /usr/share/keyrings/llvm.gpg /tmp/llvm.key && \
    echo 'deb [signed-by=/usr/share/keyrings/llvm.gpg] http://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-19 main' > /etc/apt/sources.list.d/llvm.list && \
    apt-get update && apt-get install -y --no-install-recommends clang-19 clang-tidy-19 llvm-19-dev libclang-19-dev libclang-rt-19-dev && \
    clang-tidy-19 --version | grep 'version 19.1.7' && rm -rf /var/lib/apt/lists/* /tmp/llvm.key && \
    \
    curl --cacert /run/secrets/system_ca --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs -o /tmp/rustup.sh && \
    CURL_CA_BUNDLE=/run/secrets/system_ca SSL_CERT_FILE=/run/secrets/system_ca sh /tmp/rustup.sh -y --default-toolchain 1.99.1 --profile minimal --component clippy --component rustfmt && rm /tmp/rustup.sh && \
    CARGO_HTTP_CAINFO=/run/secrets/system_ca cargo fetch --locked && \
    curl --cacert /run/secrets/system_ca -fsSL https://json.schemastore.org/sarif-2.1.0.json -o sarif.json && echo 'c96eb2d311c37b0a38cbd18c52d79a68f778bbd2d831abed7412d9850740f785  sarif.json' | sha256sum -c -
# ESBMC is externally mounted, never bundled.
