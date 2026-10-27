# Formal verification

## ESBMC 8.6.0 or later

The validated reference is 8.6.0; other supported releases record that output
interpretation is unvalidated.

The adapter selects Z3, 32/64-bit width, overflow checking and default unwinding
assertions. Select one compilation entry using `--variant` when necessary.
Only reviewed argument translations are accepted; semantic options are never
silently dropped. BMC is the default; `--method induction` selects native
k-induction with `--max-k-step` equal to `--unwind` (default 8).

```sh
safety prove harness/component.c --engine esbmc --unwind 8 --bits 32
safety prove harness/component.cpp --engine esbmc --method induction --unwind 16
```

`PROVEN` requires successful native termination, enumerated passed properties,
no warnings, and no failed completeness checks. The claim is about the recorded
sequential source/library model. BMC bounds remain explicit; successful
unwinding checks establish completeness only in that model. Induction search
limits and native base/forward/inductive phase logs remain attached. A bounded
pass with incomplete unwinding is `UNKNOWN`. Solver/parser/time/output failures
are `FAILED`; missing property enumeration cannot produce a proof. Counterexamples
are retained as native JSON and logs; they need investigation before being
classified as production defects. MCP `safety.get_run` returns a bounded trace
excerpt and its full local evidence reference.

## Frama-C 25.0 (Manganese) or later

Releases other than the validated `25.0-beta (Manganese)`, including 25.0
final, record that output interpretation is unvalidated. A recognised release
below 25.0 is unsupported; an unrecognised version is a failure.
The validated reference package is Debian `frama-c-base=20220511-manganese-3+b4`.
C only; a selected complete analysis unit must contain the entry function.
The default is Eva; WP is selected explicitly and uses Qed for supplied ACSL
contracts. Other external prover commands are not exposed.

```sh
safety prove harness/component.c --engine frama-c --method eva --entry main
safety prove src/contracts.c --engine frama-c --method wp --entry identity
```

The shared runner invokes allowlisted Clang 19 to preprocess into run-local
`analysis.i`, preserving ACSL and copyright comments. It uses the installed
Frama-C libc model and explicit x86_32/x86_64 machine model. Frama-C consumes
the `.i` directly; no project-controlled shell preprocessor is executed.
Definitions, include options and dialect are translated conservatively; other
semantic options are incomplete. Native CSV property statuses are requested
**after** analysis using `-then -report`. Only a native `Valid` result with clean
termination can be `PROVEN`. `Considered valid` is an assumption, not proof;
invalid-or-unreachable and unproved properties remain `UNKNOWN`. No selected
properties is `NOT_CHECKED`. Native reports, WP JSON, alarms, preprocessing,
dependency data, command arguments and model assumptions are preserved.

Eva analyzes sequential C. Concurrency, interrupts, environment fidelity and
unsupported constructs need an explicitly reviewed model; source-tool success
never establishes those target properties. Unsupported tool versions, C++,
flags or diagnostic/report failures are incomplete.

## Harness metadata

`--harness harness/component.yaml` accepts the strict format in
[the example](../examples/mixed-demo/harness/navigation_harness.yaml). It records
target/property, input assumptions, environment model, build configuration,
requirements and analyzer engine/method/entry/word width/bounds. The requested
source and configuration must match that metadata. `verification.json` is
integrity-bound with the run, and supplied assumptions accompany properties.
The toolkit validates structure and consistency; it does not approve assumptions
or authenticate a reviewer. Human reviews are supplied separately through
`evidence.reviews` and never change generated outcomes.

Real integration fixtures cover C/C++ safe and violated assertions, insufficient
bounds, induction, harness mismatch, Eva valid/unknown and WP contracts.
External analyzers are never included in toolkit binary packages. Review the
upstream ESBMC bundle's solver-specific licenses before redistribution.
