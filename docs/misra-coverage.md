# MISRA coverage and original checks

No exact MISRA guideline mapping is validated. The user authorized proceeding
with unmapped original project-policy checks. MISRA C:2012 / C++:2023 identifiers
are selectable metadata; licensed guideline inventories and reviewed mappings
remain outstanding. No standard descriptions are distributed or reconstructed.

The native LLVM 19.1.7 check `safety-prohibited-goto` reports direct `GotoStmt`
AST nodes. It uses the keyword's source location and offers no automatic fix.
It works in C and C++, including macro expansion and instantiated templates.
GNU indirect goto is outside its behavior. This check is **not** claimed to fully
or partially implement any specific MISRA guideline.

To enable it, configure the absolute module path and an explicit executable /
module allowlist. Native code must be a reviewed build, not an untrusted library
from the analyzed project:

```yaml
tools:
  clang: true
  clang_module: /trusted/SafetyMisra.so
  allowlist:
    - /trusted/clang-tidy-19
    - /trusted/SafetyMisra.so
```

The manifest records module path/hash and its unvalidated mapping status. Six
native fixtures cover positive, negative, boundary, macro, C++ overload/template
and invalid input; a real CLI integration test also loads the module.

`scan --profile misra-c-2012` or `misra-cpp-2023` performs available static checks
and records the requested profile. A selected MISRA edition forces incomplete
status because enforcement mappings remain unvalidated. `misra-drafts.json`
contains incomplete draft GEP/GCS information and outstanding work. It does not
assign guideline compliance from analyzer silence, invent guideline inventories,
approve deviations or implement GRP/deviation management.
