#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
# Download (or take a pre-downloaded copy of) the pinned upstream ESBMC
# release, verify its SHA-256 and unpack it into DEST. Idempotent and safe to
# run concurrently. All output goes to stderr so callers can keep stdout clean
# (the container entrypoint uses stdout for MCP).
#
#   fetch_esbmc.sh --dest DIR --url URL --sha256 HEX [--zip FILE]
#
# The upstream bundle contains mixed solver/component licenses and is not
# redistributed by this project; review them before use (docs/dependencies.md).
set -eu
dest='' url='' sha256='' zip=''
while [ $# -gt 0 ]; do
  case "$1" in
    --dest) dest=$2; shift 2 ;;
    --url) url=$2; shift 2 ;;
    --sha256) sha256=$2; shift 2 ;;
    --zip) zip=$2; shift 2 ;;
    *) echo "fetch_esbmc: unknown argument $1" >&2; exit 2 ;;
  esac
done
if [ -z "$dest" ] || [ -z "$sha256" ] || { [ -z "$url" ] && [ -z "$zip" ]; }; then
  echo "fetch_esbmc: --dest, --sha256 and --url or --zip are required" >&2
  exit 2
fi
case "$sha256" in *[!0-9a-f]*|'') echo "fetch_esbmc: invalid SHA-256" >&2; exit 2 ;; esac

installed() { [ -x "$dest/bin/esbmc" ] && "$dest/bin/esbmc" --version >/dev/null 2>&1; }
installed && exit 0

mkdir -p "$dest"
# Serialise concurrent first runs (for example two containers sharing a volume).
if command -v flock >/dev/null 2>&1; then
  exec 9>"$dest/.lock"
  flock 9
  installed && exit 0
fi
work=$(mktemp -d "$dest/.download.XXXXXX")
trap 'rm -rf "$work"' EXIT INT TERM
if [ -n "$zip" ]; then
  cp "$zip" "$work/esbmc.zip"
else
  echo "fetch_esbmc: downloading $url" >&2
  curl --proto '=https' --tlsv1.2 -fsSL "$url" -o "$work/esbmc.zip"
fi
echo "$sha256  $work/esbmc.zip" | sha256sum -c - >&2
unzip -q "$work/esbmc.zip" -d "$work/unpacked"
[ -x "$work/unpacked/release/bin/esbmc" ] || { echo "fetch_esbmc: unexpected archive layout" >&2; exit 1; }
rm -rf "$dest/release"
mv "$work/unpacked/release" "$dest/release"
ln -sfn release/bin "$dest/bin"
echo "fetch_esbmc: installed $("$dest/bin/esbmc" --version 2>&1 | head -n 1) in $dest" >&2
