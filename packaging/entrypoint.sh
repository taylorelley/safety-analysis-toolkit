#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
# Container entrypoint. With no arguments, or "mcp", serve MCP over stdio for
# the workspace (SAFETY_WORKSPACE, default the working directory). Any other
# arguments run that safety command. stdout carries MCP only; every message
# from this script goes to stderr.
#
#   SAFETY_ESBMC=auto|skip   fetch the pinned ESBMC release on first start (auto)
#   SAFETY_ESBMC_ZIP=FILE    use a pre-downloaded ESBMC archive (still hash-checked)
#   SAFETY_AUTO_INIT=1|0     create safety.yaml when the workspace has none (1)
set -eu
log() { echo "safety-container: $*" >&2; }
workspace=${SAFETY_WORKSPACE:-$(pwd)}
module=${SAFETY_MODULE:-/opt/safety/lib/safety/SafetyMisra.so}
esbmc_env=${SAFETY_ESBMC_ENV:-/opt/safety/share/safety/esbmc.env}
SAFETY_ESBMC_DIR=${SAFETY_ESBMC_DIR:-/opt/esbmc}
[ $# -eq 0 ] && set -- mcp

ensure_esbmc() {
  if [ "${SAFETY_ESBMC:-auto}" = skip ]; then
    log "ESBMC fetch skipped (SAFETY_ESBMC=skip)"
    return 0
  fi
  # shellcheck source=/dev/null
  . "$esbmc_env"
  if [ -n "${SAFETY_ESBMC_ZIP:-}" ]; then
    set -- --zip "$SAFETY_ESBMC_ZIP"
  else
    set -- --url "$ESBMC_URL"
  fi
  if ! safety-fetch-esbmc --dest "$SAFETY_ESBMC_DIR" --sha256 "$ESBMC_SHA256" "$@"; then
    log "ESBMC is unavailable (no network or failed verification); ESBMC proofs will report it as missing"
  fi
}

ensure_config() {
  [ -f "$workspace/safety.yaml" ] && return 0
  if [ "${SAFETY_AUTO_INIT:-1}" = 0 ]; then
    log "no safety.yaml in $workspace and SAFETY_AUTO_INIT=0"
    exit 2
  fi
  # init resolves the allowlist from PATH, so it runs after the ESBMC fetch.
  safety init --path "$workspace" --module "$module" >&2
  sed -i 's/^  frama_c: false$/  frama_c: true/' "$workspace/safety.yaml"
  if [ -x "$SAFETY_ESBMC_DIR/bin/esbmc" ]; then
    sed -i 's/^  esbmc: false$/  esbmc: true/' "$workspace/safety.yaml"
  fi
  log "created $workspace/safety.yaml for this container's tool paths; review it before relying on results"
}

case "$1" in
  mcp)
    shift
    ensure_esbmc
    ensure_config
    exec safety mcp --path "$workspace" "$@"
    ;;
  *)
    ensure_esbmc
    exec safety "$@"
    ;;
esac
