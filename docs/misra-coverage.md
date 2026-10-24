# MISRA coverage and original checks

`profiles/misra-c-2012.yaml` inventories every guideline of the MISRA C:2012
first edition (March 2013): 16 directives (Dir 1.1-4.13) and 143 rules (Rule
1.1-22.6). Each record carries the guideline's identifier, category, decidability,
analysis scope and C90/C99 applicability, taken from a user-supplied licensed copy,
and its automated assessment mapping. No headline or other standard text is
reproduced; diagnostic messages are original wording. Amendments and technical
corrigenda are not included.

| Category | Directives | Rules |
| --- | --- | --- |
| Mandatory | 0 | 10 |
| Required | 9 | 101 |
| Advisory | 7 | 32 |

## Automated assessment

At the user's request every guideline that can be checked from source now has
an automated check. 157 guidelines are `partial`; Dir 3.1 (requirements
traceability, a project process) and Rule 8.6 (one external definition, which
needs whole-program link analysis) remain `manual`. No guideline is `supported`:
each check covers a stated subset of its guideline and every guideline still
requires manual review.

| Section | Guidelines | SafetyMisra check | Clang analyzer/compiler only | Manual |
| --- | --- | --- | --- | --- |
| Directives 1-4 | 16 | 11 | 4 (Dir 2.1, 4.1, 4.11, 4.13) | 1 (Dir 3.1) |
| Rules 1-9 | 48 | 45 | 2 (Rule 1.3, 9.1) | 1 (Rule 8.6) |
| Rules 10-19 | 63 | 63 | 0 | 0 |
| Rules 20-22 | 32 | 29 | 3 (Rule 22.1, 22.2, 22.6) | 0 |

The `SafetyMisra` Clang-Tidy module registers one check per guideline, named
`misra-c2012-dir-X-Y` or `misra-c2012-rule-X-Y` (148 checks). Some guidelines
also map to Clang static analyzer checks (for example uninitialized reads,
stream and heap misuse, stack-address escape, shift amounts) or to
`clang-diagnostic-error` for compile errors. The profile lists the exact checks
for each guideline in `automated_checks`, and its `limitations` state what the
check does not cover. Typical limits:

- Rules marked *system* in the standard (for example 5.6-5.9, 8.7, 8.9, 17.2)
  are checked within one translation unit only.
- Undecidable rules (for example 13.2, 14.3, 18.1-18.3, 19.1) report the
  statically evident cases; path-dependent cases need review or the analyzer.
- Directives produce evidence for a reviewer (for example assembly use,
  comment heuristics, include guards); documentation they require stays manual.
- Rule 5.1 assumes 31 significant characters for external identifiers, and
  Rules 5.2-5.5 use 31 (C90) or 63 (C99) characters.
- Dir 4.7 uses a configurable `ErrorFunctions` list of library functions.

The checks implement the essential type model of Appendix D for Rules
10.1-10.8, 12.2, 14.4 and 16.7, including the signed and unsigned type of lowest
rank for constants and bit-fields. Checks apply to C translation units only.

## Test evidence and review status

`clang/tests/misra/` holds 164 known-answer fixtures. Each fixture names the
checks it exercises; every line marked `NONCOMPLIANT` must receive a diagnostic
and every other line must not, so each fixture tests positives and negatives.
`clang/tests/misra.py` (CTest `misra-c2012`) also requires a fixture for every
registered check and runs all checks together over every fixture, including
one with compile errors, to detect crashes. The fixtures are the profile's
`test_evidence`; for the bundled profile these paths refer to the toolkit
repository, not to the analysed project.

Each automated mapping records `reviewed_by: Implementation author
(AI-assisted); independent review pending`. The mappings were written against
the user-supplied licensed copy but have not been reviewed by an independent
qualified person. Treat diagnostics as candidate violations and absence of
diagnostics as no evidence of compliance. A project that completes its own
review records it in a registry (below), which replaces the bundled profile.

The MISRA C++:2023 manifest still has an empty, explicitly incomplete inventory.

## Running the checks

Select a reviewed native module:

```yaml
tools:
  clang_module: /trusted/lib/safety/SafetyMisra.so
  allowlist:
    - /trusted/bin/clang-tidy-19
    - /trusted/lib/safety/SafetyMisra.so
coding:
  c: MISRA-C:2012
```

`scan` then enables `safety-*` and `misra-c2012-*` alongside the analyzer
checks. Findings carry the check name as `rule_id`; the GEP/GCS draft links them
to guidelines through `automated_checks` and records whether every mapped check
ran on a complete scan. The scan stays `incomplete` while a MISRA edition is
configured, because mappings are partial and manual guidelines remain.

## Original policy checks

| Original check | Implemented behavior | Scope limit |
| --- | --- | --- |
| safety-prohibited-goto | Direct Clang GotoStmt | GNU indirect goto excluded |
| safety-unsafe-library-call | Direct global C-linkage gets/longjmp calls | Indirect calls and wrappers excluded |
| safety-repeated-macro-parameter | Repeated parameter identifiers in function-like macro bodies | Syntactic repetition; does not prove runtime multiple evaluation |

These checks stay unmapped project policy. All checks use AST matchers,
preprocessor callbacks or comment handlers, emit original diagnostic text and
provide no automatic fixes.

## Project-supplied registry and process records

`coding.registries` accepts paths to reviewed edition manifests using the strict
structure of the bundled profiles. Rule metadata distinguishes supported,
partial, manual, not_implemented and not_applicable, with implementation,
automated check IDs, manual-review flag and limitations. Supported/partial
claims require a mapping-review source reference, reviewer, timestamp and
test-evidence paths that exist in the project. Not-applicable needs documented
project conditions. Optional `category`, `decidability`, `analysis_scope` and
`applies_to` fields record guideline classification. Unknown editions, duplicate
IDs and unsupported fields are rejected. This validates supplied records; it
cannot authenticate a licensed standards review or replace that review.

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
GCS never derives compliance from analyzer silence. Actual check selection and
translation-unit completeness determine whether an execution is recorded as
performed; manual work and partial mappings remain outstanding.
