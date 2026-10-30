#!/usr/bin/env bash
# SPDX-License-Identifier: Apache-2.0
# Install every dependency (LLVM/Clang 19.1, Rust, Frama-C with Eva and WP,
# ESBMC), build the toolkit from source, test it and install it under PREFIX.
# Each step is skipped when a suitable version is already present; sudo is used
# only for missing apt packages. Supports Debian 12 and Ubuntu 22.04/24.04.
#
#   scripts/bootstrap.sh [--prefix DIR] [--frama-c auto|apt|opam|skip]
#                        [--skip-esbmc] [--esbmc-zip FILE] [--skip-tests]
set -euo pipefail

repo=$(cd -- "$(dirname -- "$0")/.." && pwd)
prefix="$HOME/.local"
frama_mode=auto
skip_esbmc=0
esbmc_zip=
skip_tests=0
while [ $# -gt 0 ]; do
  case "$1" in
    --prefix) prefix=$2; shift 2 ;;
    --frama-c) frama_mode=$2; shift 2 ;;
    --skip-esbmc) skip_esbmc=1; shift ;;
    --esbmc-zip) esbmc_zip=$2; shift 2 ;;
    --skip-tests) skip_tests=1; shift ;;
    -h|--help) sed -n '2,11p' "$0"; exit 0 ;;
    *) echo "bootstrap: unknown argument $1" >&2; exit 2 ;;
  esac
done
case "$prefix" in /*) ;; *) echo "bootstrap: --prefix must be absolute" >&2; exit 2 ;; esac
case "$frama_mode" in auto|apt|opam|skip) ;; *) echo "bootstrap: --frama-c must be auto, apt, opam or skip" >&2; exit 2 ;; esac

step() { printf '\n==> %s\n' "$*"; }
# Pinned versions and downloads come from the single source of truth.
pin() { python3 -c "import json,sys;d=json.load(open(sys.argv[1]));[d:=d[k] for k in sys.argv[2].split('.')];print(d)" "$repo/docs/tool-versions.json" "$1"; }
version_ge() { [ "$(printf '%s\n%s\n' "$2" "$1" | sort -V | head -n 1)" = "$2" ]; }
missing_packages() {
  local p
  for p in "$@"; do dpkg-query -W -f='${Status}' "$p" 2>/dev/null | grep -q 'install ok installed' || printf '%s ' "$p"; done
}
apt_install() {
  local needed
  needed=$(missing_packages "$@")
  [ -z "$needed" ] && return 0
  echo "Installing apt packages: $needed"
  local sudo=; [ "$(id -u)" -ne 0 ] && sudo=sudo
  $sudo apt-get update
  # shellcheck disable=SC2086
  DEBIAN_FRONTEND=noninteractive $sudo apt-get install -y --no-install-recommends $needed
}

command -v apt-get >/dev/null || { echo "bootstrap: apt-get not found; see docs/installation.md for manual setup" >&2; exit 1; }
# shellcheck disable=SC1091
. /etc/os-release
codename=${VERSION_CODENAME:-}
echo "Host: ${PRETTY_NAME:-unknown}; prefix: $prefix"

step "Base build tools"
apt_install ca-certificates curl gnupg git make cmake g++ python3 python3-jsonschema unzip xz-utils
min_cmake=$(pin minimum.cmake)
cmake_version=$(cmake --version | sed -n '1s/.* //p')
version_ge "$cmake_version" "$min_cmake" || { echo "bootstrap: CMake $cmake_version is older than $min_cmake" >&2; exit 1; }

step "LLVM/Clang 19.1"
llvm_min=$(pin minimum.llvm)
llvm_ok() {
  local v
  v=$(clang-tidy-19 --version 2>/dev/null | sed -n 's/.*version \(19\.1\.[0-9]*\).*/\1/p') || true
  [ -n "$v" ] && version_ge "$v" "$llvm_min" && [ -f /usr/lib/llvm-19/lib/cmake/clang/ClangConfig.cmake ]
}
llvm_packages=(clang-19 clang-tidy-19 llvm-19 llvm-19-dev libclang-19-dev libclang-rt-19-dev)
if ! llvm_ok; then
  candidate=$(apt-cache policy clang-tidy-19 2>/dev/null | sed -n 's/.*Candidate: \(.*\)/\1/p')
  if [ -z "$candidate" ] || [ "$candidate" = "(none)" ] || ! printf '%s' "$candidate" | grep -q '19\.1\.'; then
    echo "Adding apt.llvm.org for $codename (signing key fingerprint is verified)"
    [ -n "$codename" ] || { echo "bootstrap: unknown distribution codename" >&2; exit 1; }
    key=$(mktemp)
    curl --proto '=https' --tlsv1.2 -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key -o "$key"
    gpg --show-keys --with-colons "$key" | grep -q '6084F3CF814B57C1CF12EFD515CF4D18AF4F7421' || { echo "bootstrap: unexpected LLVM signing key" >&2; exit 1; }
    sudo=; [ "$(id -u)" -ne 0 ] && sudo=sudo
    gpg --dearmor < "$key" | $sudo tee /usr/share/keyrings/llvm.gpg >/dev/null
    echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/$codename/ llvm-toolchain-$codename-19 main" | $sudo tee /etc/apt/sources.list.d/llvm-19.list >/dev/null
    rm -f "$key"
  fi
  apt_install "${llvm_packages[@]}"
  llvm_ok || { echo "bootstrap: LLVM 19.1 >= $llvm_min with development files is still unavailable" >&2; exit 1; }
fi
echo "Using $(clang-tidy-19 --version | sed -n 's/^ *//;/LLVM version/p')"

step "Rust"
rust_min=$(pin minimum.rust)
[ -x "$HOME/.cargo/bin/cargo" ] && export PATH="$HOME/.cargo/bin:$PATH"
rust_version=$(rustc --version 2>/dev/null | awk '{print $2}') || true
if [ -z "$rust_version" ] || ! version_ge "$rust_version" "$rust_min"; then
  if command -v rustup >/dev/null; then
    rustup toolchain install stable --profile minimal && rustup default stable
  else
    installer=$(mktemp)
    curl --proto '=https' --tlsv1.2 -fsSL https://sh.rustup.rs -o "$installer"
    sh "$installer" -y --profile minimal --default-toolchain stable
    rm -f "$installer"
  fi
  export PATH="$HOME/.cargo/bin:$PATH"
fi
echo "Using $(rustc --version)"

step "Frama-C with Eva and WP"
frama_ok() {
  local exe=$1 v
  v=$("$exe" -version 2>/dev/null | sed -n '1s/^\([0-9][0-9.]*\).*/\1/p') || true
  [ -n "$v" ] && version_ge "$v" "$(pin minimum.frama_c)" &&
    "$exe" -plugins 2>/dev/null | awk '{print $1}' | grep -qx WP &&
    "$exe" -plugins 2>/dev/null | awk '{print $1}' | grep -qx Eva
}
frama_bin=
if [ "$frama_mode" != skip ]; then
  candidates=("$prefix/bin/frama-c" "$(command -v frama-c || true)")
  # Reuse any existing opam switch that already provides a suitable Frama-C.
  if command -v opam >/dev/null && [ -d "$HOME/.opam" ]; then
    for existing in $(opam switch list --short 2>/dev/null); do
      candidates+=("$(opam var --switch "$existing" bin 2>/dev/null)/frama-c")
    done
  fi
  for candidate in "${candidates[@]}"; do
    if [ -n "$candidate" ] && [ -x "$candidate" ] && frama_ok "$candidate"; then frama_bin=$candidate; break; fi
  done
  if [ -z "$frama_bin" ] && { [ "$frama_mode" = apt ] || { [ "$frama_mode" = auto ] && [ "${ID:-}" = debian ]; }; }; then
    # Debian's frama-c-base includes WP; Ubuntu's build omits it.
    apt_install frama-c-base
    frama_ok /usr/bin/frama-c && frama_bin=/usr/bin/frama-c
    [ -n "$frama_bin" ] || [ "$frama_mode" = auto ] || { echo "bootstrap: apt Frama-C lacks WP or Eva; use --frama-c opam" >&2; exit 1; }
  fi
  if [ -z "$frama_bin" ]; then
    echo "Building Frama-C from opam (WP is not in this distribution's package); this takes a while"
    apt_install opam bubblewrap patch autoconf graphviz pkg-config libgmp-dev zlib1g-dev libexpat1-dev \
      libcairo2-dev libgtk-3-dev libgtksourceview-3.0-dev
    switch=safety-frama-c
    [ -d "$HOME/.opam" ] || opam init --bare -n -y
    opam switch list --short 2>/dev/null | grep -qx "$switch" ||
      opam switch create "$switch" "$(pin frama_c_opam_compiler)" -y
    opam install --switch "$switch" -y "$(pin frama_c_opam_package)"
    frama_bin="$(opam var --switch "$switch" bin)/frama-c"
    frama_ok "$frama_bin" || { echo "bootstrap: opam Frama-C lacks WP or Eva" >&2; exit 1; }
  fi
  install -d "$prefix/bin"
  [ "$frama_bin" = "$prefix/bin/frama-c" ] || ln -sfn "$frama_bin" "$prefix/bin/frama-c"
  echo "Using Frama-C $("$frama_bin" -version) from $frama_bin"
fi

step "ESBMC"
esbmc_dir="$prefix/opt/esbmc-$(pin esbmc)"
if [ "$skip_esbmc" -eq 0 ]; then
  echo "The upstream ESBMC bundle contains mixed solver/component licenses; review them before use."
  args=(--dest "$esbmc_dir" --sha256 "$(pin esbmc_download_sha256)")
  if [ -n "$esbmc_zip" ]; then args+=(--zip "$esbmc_zip"); else args+=(--url "$(pin esbmc_setup_download)"); fi
  "$repo/scripts/fetch_esbmc.sh" "${args[@]}"
  install -d "$prefix/bin"
  ln -sfn "$esbmc_dir/bin/esbmc" "$prefix/bin/esbmc"
  echo "Using $("$prefix/bin/esbmc" --version 2>&1 | head -n 1)"
fi

step "Build and install the toolkit"
cd "$repo"
cargo build --locked --release
cmake -S clang -B build/clang -DCMAKE_BUILD_TYPE=Release
cmake --build build/clang --parallel "$(nproc)"
if [ "$skip_tests" -eq 0 ]; then
  cargo test --locked
  ctest --test-dir build/clang --output-on-failure
fi
scripts/install.sh "$prefix"

step "Self-check"
check=$(mktemp -d)
trap 'rm -rf "$check"' EXIT
printf 'int value(int *p) { return p ? *p : 0; }\n' > "$check/check.c"
printf '[{"directory":"%s","file":"check.c","arguments":["clang-19","-std=c11","-c","check.c"]}]\n' "$check" > "$check/compile_commands.json"
export PATH="$prefix/bin:$PATH"
safety init --path "$check" --module "$prefix/lib/safety/SafetyMisra.so" >/dev/null
sed -i 's#compile_commands: build/compile_commands.json#compile_commands: compile_commands.json#' "$check/safety.yaml"
[ "$frama_mode" = skip ] || sed -i 's/^  frama_c: false/  frama_c: true/' "$check/safety.yaml"
[ "$skip_esbmc" -eq 1 ] || sed -i 's/^  esbmc: false/  esbmc: true/' "$check/safety.yaml"
if ! safety doctor "$check" > "$check/doctor.json"; then
  python3 -c "import json,sys;[print('  '+l) for l in json.load(open(sys.argv[1]))['limitations']]" "$check/doctor.json" >&2
  echo "bootstrap: safety doctor reported missing capabilities" >&2
  exit 1
fi
python3 -c "import json,sys;[print('  note: '+l) for l in json.load(open(sys.argv[1]))['limitations'] if any(t in l for t in ('differs','WP','unavailable'))]" "$check/doctor.json"
# $PATH is printed literally for the user to paste into their shell profile.
# shellcheck disable=SC2016
printf '\nInstalled. Add %s/bin to PATH ahead of system directories, so this Frama-C and ESBMC are used:\n  export PATH="%s/bin:$PATH"\n' "$prefix" "$prefix"
