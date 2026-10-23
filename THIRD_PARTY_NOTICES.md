# Third-party notices

Original repository code is Apache-2.0. No MISRA standards text is included.
Rust dependencies retain their respective copyright and license notices. Consult
Cargo.lock and the generated dependency inventory before redistributing binaries.
No analyzer executables, native libraries or container images are redistributed.

LLVM/Clang 19.1.7: Apache-2.0 WITH LLVM-exception (and component-specific notices).
Rust MCP SDK (rmcp): Apache-2.0. See each upstream project's licenses for actual
redistribution obligations. Process invocation does not waive license obligations.

The official SARIF 2.1.0 schema is fetched only into an ignored local cache for
validation and is not redistributed here. It is OASIS material under the OASIS
repository/specification terms. Its use is separate from the license on original
schemas in this repository.

Dependency inventory generation records declared SPDX expressions; it is not a
legal opinion or completed redistribution approval. The packaging workflow enforces a permissive SPDX selection policy and
aggregates license/notice texts for the reachable locked Linux Rust graph.
Review `share/safety/license-inventory/inventory.json` and retained notices in a
package. This review does not approve redistribution of external analyzers.

Frama-C Manganese model headers appearing in executed validation evidence are
LGPL-2.1, copyright CEA and respective contributors, with original comments
preserved. See `validation/third-party/` for the relevant license/copyright
notices. Those model-derived evidence artifacts are not Apache-2.0 original
code and are not included in toolkit binary archives.
