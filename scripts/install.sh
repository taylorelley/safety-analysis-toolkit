#!/bin/sh
# SPDX-License-Identifier: Apache-2.0
set -eu
safety_prefix=${1:-"$HOME/.local"}
case "$safety_prefix" in /*) ;; *) echo 'Installation prefix must be absolute' >&2; exit 2;; esac
cargo build --locked --release
cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release
cmake --build build/clang --parallel 2
python3 scripts/license_inventory.py build/license-inventory
install -d "$safety_prefix/bin" "$safety_prefix/lib/safety" "$safety_prefix/share/safety"
install -m 755 target/release/safety "$safety_prefix/bin/"
cmake --install build/clang --prefix "$safety_prefix"
cp -R LICENSE THIRD_PARTY_NOTICES.md profiles schemas skills docs build/license-inventory "$safety_prefix/share/safety/"
"$safety_prefix/bin/safety" --version
printf 'Installed original toolkit artifacts at %s; external analyzers are not bundled.\n' "$safety_prefix"
