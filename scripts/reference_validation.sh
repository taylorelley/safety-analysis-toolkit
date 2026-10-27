#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
# Setup builds the image with network; all validation runs without networking.
set -eu
safety_repository=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
: "${SAFETY_ESBMC_DIR:?Set SAFETY_ESBMC_DIR to an externally reviewed ESBMC 8.6.0 (or later) bin directory}"
test -x "$SAFETY_ESBMC_DIR/esbmc"
cd "$safety_repository"
mkdir -p build/reference-results validation/completion
if [ "${SAFETY_REFERENCE_SKIP_SETUP:-0}" != 1 ]; then
env -u DOCKER_HOST -u DOCKER_CONTEXT -u DOCKER_TLS -u DOCKER_TLS_VERIFY -u DOCKER_CERT_PATH \
  docker --host=unix:///var/run/docker.sock build \
  --secret id=system_ca,src=/etc/ssl/certs/ca-certificates.crt \
  -f packaging/reference.Dockerfile -t safety-reference:local .
fi
env -u DOCKER_HOST -u DOCKER_CONTEXT -u DOCKER_TLS -u DOCKER_TLS_VERIFY -u DOCKER_CERT_PATH \
  docker --host=unix:///var/run/docker.sock run --rm --network none \
  -v "$safety_repository:/input:ro" \
  -v "$SAFETY_ESBMC_DIR:/opt/external:ro" \
  -v "$safety_repository/build/reference-results:/results" \
  safety-reference:local sh -ec '
    mkdir -p /work/safety
    tar -C /input --exclude=target --exclude=build --exclude=.git --exclude=node_modules --exclude=validation --exclude=.cache -cf - . | tar -C /work/safety -xf -
    cd /work/safety
    mkdir .cache
    cp /opt/safety-cache/sarif.json .cache/sarif-2.1.0.json
    export CARGO_NET_OFFLINE=true CARGO_INCREMENTAL=0
    export PATH=/work/safety/build/clang:/opt/external:$PATH
    make full-validation
    scripts/install.sh /opt/safety-install
    /opt/safety-install/bin/safety --version
    /opt/safety-install/bin/safety-source-index --version
    python3 scripts/configure_demo.py
    python3 scripts/benchmark.py
    cp validation/completion/benchmark.json /results/benchmark.json
    python3 scripts/package.py
    sha256sum build/safety-*.tar.gz > /tmp/package-first.sha256
    python3 scripts/package.py
    sha256sum -c /tmp/package-first.sha256
    cp build/safety-*.tar.gz* /results/
    cp -R build/license-inventory /results/
    /opt/safety-install/bin/safety doctor examples/embedded-c
    printf "OFFLINE_REFERENCE_VALIDATION_PASSED\n"
  '
