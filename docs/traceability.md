# AST traceability

Build `safety-source-index` with the pinned native build and install it on a
trusted PATH. `safety trace` invokes it for each supported compilation entry,
preserves Clang USRs, declarations/definitions, locations, names, comments,
template-parent identities and build variants. It does not detect functions with
regular expressions. Original JSONL and a combined `trace.json` remain evidence.

Default manifest: `trace.yaml`; override with `trace.manifest` in safety.yaml.
JSON is accepted as YAML. Markdown manifests use YAML frontmatter delimited by
`---` lines. The graph is explicit:

```yaml
version: 1
baseline: BL-1
nodes:
  - {id: R1, kind: requirement, root: true}
  - {id: D1, kind: design}
  - {id: T1, kind: test}
links:
  - {from: R1, to: D1}
  - {from: R1, to: T1}
```

Code nodes use canonical Clang USRs. Function comments with `@req R1` and
`@design D1` add explicit links to the associated symbol; `@test T1` identifies
a test function rather than production implementation. `@baseline BL-1` is
checked against the manifest. Unknown/mistyped identifiers are findings. Macro
symbol annotation mappings are flagged as ambiguous, requiring explicit links.

Allowed node kinds: requirement, design, code, test, finding, evidence,
change_request, problem_report. Link kinds follow their endpoint types. Broken
references, duplicate IDs, invalid relationship types, missing requirement
parents, missing implementations/tests, untraced functions, tests without known
requirements and baseline mismatches are reported. Requirement reachability is
transitive through valid requirement/design links.

Scope limitations: call-edge extraction, comprehensive template specialization
relationships, cross-variant annotation reconciliation and external policy's
additional annotations are not implemented. Raw Clang comment attachment is
used; no semantic interpretation is inferred. Trace links do not establish that
an implementation satisfies a requirement or that a test is adequate.
