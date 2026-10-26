# Dependency and license inventory

Original toolkit code: Apache-2.0. Cargo.toml sets minimum crate versions and Cargo.lock
records the resolved graph; native extensions target LLVM/Clang 19.1.7 or a later
19.1.x release. `scripts/license_inventory.py` traverses the
locked Linux dependency graph, selects an allowed SPDX license alternative and
copies package license/notice files into the binary package. It fails closed
for unknown/disallowed expressions or missing license texts. This is an
engineering redistribution policy, not a legal opinion.

| Rust dependency | Version | Declared license | Selected alternative |
| --- | --- | --- | --- |
| anstream | 1.0.0 | MIT OR Apache-2.0 | MIT |
| anstyle | 1.0.14 | MIT OR Apache-2.0 | MIT |
| anstyle-parse | 1.0.0 | MIT OR Apache-2.0 | MIT |
| anstyle-query | 1.1.5 | MIT OR Apache-2.0 | MIT |
| anyhow | 1.0.104 | MIT OR Apache-2.0 | MIT |
| autocfg | 1.5.1 | Apache-2.0 OR MIT | Apache-2.0 |
| bitflags | 2.13.2 | MIT OR Apache-2.0 | MIT |
| block-buffer | 0.10.4 | MIT OR Apache-2.0 | MIT |
| bytes | 1.12.1 | MIT | MIT |
| cfg-if | 1.0.5 | MIT OR Apache-2.0 | MIT |
| chrono | 0.4.45 | MIT OR Apache-2.0 | MIT |
| clap | 4.6.7 | MIT OR Apache-2.0 | MIT |
| clap_builder | 4.6.7 | MIT OR Apache-2.0 | MIT |
| clap_derive | 4.6.7 | MIT OR Apache-2.0 | MIT |
| clap_lex | 1.1.1 | MIT OR Apache-2.0 | MIT |
| colorchoice | 1.0.5 | MIT OR Apache-2.0 | MIT |
| cpufeatures | 0.2.17 | MIT OR Apache-2.0 | MIT |
| crypto-common | 0.1.7 | MIT OR Apache-2.0 | MIT |
| csv | 1.4.0 | Unlicense/MIT | Unlicense |
| csv-core | 0.1.13 | Unlicense/MIT | Unlicense |
| darling | 0.24.1 | MIT | MIT |
| darling_core | 0.24.1 | MIT | MIT |
| darling_macro | 0.24.1 | MIT | MIT |
| digest | 0.10.7 | MIT OR Apache-2.0 | MIT |
| dyn-clone | 1.0.20 | MIT OR Apache-2.0 | MIT |
| equivalent | 1.0.3 | Apache-2.0 OR MIT | Apache-2.0 |
| errno | 0.3.14 | MIT OR Apache-2.0 | MIT |
| fastrand | 2.5.0 | Apache-2.0 OR MIT | Apache-2.0 |
| futures | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-channel | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-core | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-executor | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-io | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-macro | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-sink | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-task | 0.3.34 | MIT OR Apache-2.0 | MIT |
| futures-util | 0.3.34 | MIT OR Apache-2.0 | MIT |
| generic-array | 0.14.7 | MIT | MIT |
| getrandom | 0.4.3 | MIT OR Apache-2.0 | MIT |
| hashbrown | 0.17.1 | MIT OR Apache-2.0 | MIT |
| heck | 0.5.0 | MIT OR Apache-2.0 | MIT |
| iana-time-zone | 0.1.65 | MIT OR Apache-2.0 | MIT |
| ident_case | 1.0.1 | MIT/Apache-2.0 | MIT |
| indexmap | 2.14.2 | Apache-2.0 OR MIT | Apache-2.0 |
| is_terminal_polyfill | 1.70.2 | MIT OR Apache-2.0 | MIT |
| itoa | 1.0.18 | MIT OR Apache-2.0 | MIT |
| libc | 0.2.191 | MIT OR Apache-2.0 | MIT |
| linux-raw-sys | 0.12.1 | Apache-2.0 WITH LLVM-exception OR Apache-2.0 OR MIT | Apache-2.0 WITH LLVM-exception |
| memchr | 2.8.3 | Unlicense OR MIT | Unlicense |
| mio | 1.2.4 | MIT | MIT |
| num-traits | 0.2.19 | MIT OR Apache-2.0 | MIT |
| once_cell | 1.21.4 | MIT OR Apache-2.0 | MIT |
| pastey | 0.2.3 | MIT OR Apache-2.0 | MIT |
| pin-project-lite | 0.2.17 | Apache-2.0 OR MIT | Apache-2.0 |
| proc-macro2 | 1.0.107 | MIT OR Apache-2.0 | MIT |
| quote | 1.0.47 | MIT OR Apache-2.0 | MIT |
| ref-cast | 1.0.27 | MIT OR Apache-2.0 | MIT |
| ref-cast-impl | 1.0.27 | MIT OR Apache-2.0 | MIT |
| rmcp | 3.7.0 | Apache-2.0 | Apache-2.0 |
| rmcp-macros | 3.7.0 | Apache-2.0 | Apache-2.0 |
| rustix | 1.1.5 | Apache-2.0 WITH LLVM-exception OR Apache-2.0 OR MIT | Apache-2.0 WITH LLVM-exception |
| ryu | 1.0.23 | Apache-2.0 OR BSL-1.0 | Apache-2.0 |
| schemars | 1.2.3 | MIT | MIT |
| schemars_derive | 1.2.3 | MIT | MIT |
| serde | 1.0.229 | MIT OR Apache-2.0 | MIT |
| serde_core | 1.0.229 | MIT OR Apache-2.0 | MIT |
| serde_derive | 1.0.229 | MIT OR Apache-2.0 | MIT |
| serde_derive_internals | 0.30.0 | MIT OR Apache-2.0 | MIT |
| serde_json | 1.0.154 | MIT OR Apache-2.0 | MIT |
| serde_yaml | 0.9.34+deprecated | MIT OR Apache-2.0 | MIT |
| sha2 | 0.10.9 | MIT OR Apache-2.0 | MIT |
| shlex | 1.3.0 | MIT OR Apache-2.0 | MIT |
| signal-hook-registry | 1.4.8 | MIT OR Apache-2.0 | MIT |
| slab | 0.4.12 | MIT | MIT |
| socket2 | 0.6.5 | MIT OR Apache-2.0 | MIT |
| strsim | 0.11.1 | MIT | MIT |
| syn | 2.0.119 | MIT OR Apache-2.0 | MIT |
| syn | 3.0.9 | MIT OR Apache-2.0 | MIT |
| tempfile | 3.27.0 | MIT OR Apache-2.0 | MIT |
| thiserror | 2.0.21 | MIT OR Apache-2.0 | MIT |
| thiserror-impl | 2.0.21 | MIT OR Apache-2.0 | MIT |
| tokio | 1.54.1 | MIT | MIT |
| tokio-macros | 2.7.2 | MIT | MIT |
| tokio-util | 0.7.20 | MIT | MIT |
| tracing | 0.1.44 | MIT | MIT |
| tracing-attributes | 0.1.31 | MIT | MIT |
| tracing-core | 0.1.36 | MIT | MIT |
| typenum | 1.20.1 | MIT OR Apache-2.0 | MIT |
| unicode-ident | 1.0.27 | (MIT OR Apache-2.0) AND Unicode-3.0 | MIT AND Unicode-3.0 |
| unsafe-libyaml | 0.2.11 | MIT | MIT |
| utf8parse | 0.2.2 | Apache-2.0 OR MIT | Apache-2.0 |
| uuid | 1.29.0 | Apache-2.0 OR MIT | Apache-2.0 |
| version_check | 0.9.5 | MIT/Apache-2.0 | MIT |
| zmij | 1.0.23 | MIT | MIT |

External executables/libraries are installed separately and are not included in
toolkit archives. LLVM/Clang uses Apache-2.0 WITH LLVM-exception and component
notices. Frama-C Manganese uses LGPL-2.1 (see package copyright and model headers).
ESBMC 8.6.0 contains Apache-2.0 modifications, CBMC lineage and solver-specific
licenses; the upstream prebuilt bundle is not approved for redistribution here.
The adapter explicitly selects Z3; that does not change the bundle's contents.

The reference container recipe is for local validation and is not a distributed
image. Before distributing an image, review all OS/analyzer licenses separately.
Codex/Pi/Node/Python are development test hosts, never dependencies of the Rust
application. The Pi example has its own pinned npm lockfile; no Node dependencies
are included in the Rust package.

serde_yaml 0.9.34 is deprecated upstream. Structured input sizes and strict
schemas are bounded; maintainers should assess migration as part of ongoing
dependency upkeep. This toolkit does not claim parser correctness or tool
qualification solely from dependency selection. All transitive source notices
collected for the validated archive are retained alongside the release inventory.
