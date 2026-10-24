# MISRA coverage and original checks

No exact MISRA guideline mapping is validated. The user authorized unmapped
original checks. No proprietary standard text or invented rule identifiers are
distributed.

`profiles/misra-c-2012.yaml` inventories every guideline of the MISRA C:2012
first edition (March 2013): 16 directives (Dir 1.1-4.13) and 143 rules (Rule
1.1-22.6). It records each guideline's identifier, category, decidability, analysis
scope and C90/C99 applicability, taken from a user-supplied licensed copy. It
does not include headlines or other standard text. Amendments and technical corrigenda are
not included. Every directive is `manual` and every rule is `not_implemented`,
and each one requires manual review. The inventory is complete, but no guideline is
automatically enforced. A project can raise a guideline to `supported` or `partial` only through a
reviewed registry. The MISRA C++:2023 manifest still has an empty, explicitly incomplete
inventory.

| Category | Directives | Rules |
| --- | --- | --- |
| Mandatory | 0 | 10 |
| Required | 9 | 101 |
| Advisory | 7 | 32 |

| Original check | Implemented behavior | Scope limit |
| --- | --- | --- |
| safety-prohibited-goto | Direct Clang GotoStmt | GNU indirect goto excluded |
| safety-unsafe-library-call | Direct global C-linkage gets/longjmp calls | Indirect calls and wrappers excluded |
| safety-repeated-macro-parameter | Repeated parameter identifier tokens in function-like macro bodies | Syntactic repetition; does not prove runtime multiple evaluation |

Checks use AST matchers/preprocessor callbacks, emit original diagnostic text
and locations and provide no automatic fixes. CTest independently exercises
positives, negatives, boundaries, macros, C++ overload/template behavior and
invalid input. Select only a reviewed native module:

```yaml
tools:
  clang_module: /trusted/lib/safety/SafetyMisra.so
  allowlist:
    - /trusted/bin/clang-tidy-19
    - /trusted/lib/safety/SafetyMisra.so
```

`scan --profile misra-c-2012` or `misra-cpp-2023` records selected editions and
remains incomplete when their guideline inventory/mappings are unvalidated.
Static defect-check success is not MISRA guideline compliance.

## Project-supplied registry and process records

`coding.registries` accepts paths to reviewed edition manifests using the strict
structure of the bundled profiles. Rule metadata distinguishes supported,
partial, manual, not_implemented and not_applicable, with implementation,
automated check IDs, manual-review flag and limitations. Supported/partial
claims require supplied mapping-review source reference, reviewer, timestamp
and existing test-evidence paths. Not-applicable needs documented project
conditions. Optional `category`, `decidability`, `analysis_scope` and
`applies_to` fields record guideline classification. Unknown editions, duplicate IDs and unsupported fields are rejected.
This validates supplied records; it cannot authenticate a licensed standards
review or replace that review. No mapping review is fabricated by this project.

`coding.deviations` accepts draft deviation records: id, edition, rule_id,
rationale, artifacts, change_request and optional external review. Unknown rules
and duplicates are rejected. Findings remain visible; the toolkit never approves
or suppresses them through a deviation. `coding.recategorization` accepts draft
GRP proposals with edition/rule, original/proposed category and rationale;
unknown or weakened categories, and original categories that differ from the
inventory, are rejected. `evidence.reviews` stores separate
externally supplied reviewer/decision/rationale/timestamp/evidence/change_record
records. These are process assertions, never toolkit-granted approval.

Every relevant run generates `misra-drafts.json` with draft GEP, GRP, GCS and
deviation summaries. GCS includes per-edition inventory size and coverage counts.
GCS never derives compliance from analyzer silence. Actual
check selection and translation-unit completeness determine whether an
execution is recorded as performed; manual work and incomplete inventories
remain outstanding. Review supplied registries against licensed editions.
