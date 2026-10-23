# Formal verification

The implemented model-checking adapter targets ESBMC **8.6.0**, selecting Z3,
explicit 32/64-bit word width (default 64), arithmetic overflow checks and a
finite unwind bound (default 8), with ESBMC's default unwinding assertions kept
enabled. A source must match exactly one compilation variant, or `--variant`
must select it. Compiler options are translated conservatively; unsupported
semantic options fail visibly. Dropping `-c` and output selection is deliberate;
macro values, include paths and dialect are preserved. There is no induction
adapter, automatic harness generation or approved environment model.

```sh
safety prove src/harness.c --engine esbmc --unwind 8 --bits 32
safety prove src/harness.cpp --engine esbmc --variant VAR-ID
```

`PROVEN` requires successful tool exit, native successful termination, enumerated
passed properties, no warnings, and default unwinding checks. Its scope remains
the recorded ESBMC source/library model and finite configured bound. Passing
unwinding checks is recorded as model completeness; it does not validate the
model against a real target. `VIOLATED` means a counterexample in that model.
Unwinding failures are `UNKNOWN`; timeouts/output-limit failures become `FAILED`.
Unrecognized output never becomes proof success. No enumerated properties means
`UNKNOWN`, not a vacuous success. Native reports and machine-readable failure
traces are retained and integrity-bound with the run.

Source assumptions and external/library models have not been independently
reviewed. Harness metadata validation and per-assumption approval tracking are
outstanding. These runs are advisory evidence. A model counterexample must be
investigated before classifying it as a production defect.

`--engine frama-c` performs capability inspection and records `NOT_CHECKED` /
incomplete. Eva/WP execution is **not implemented or validated** in this snapshot.
The development environment's Debian snapshot has no frama-c-base package. This
is a visible capability boundary, not a completed formal-analysis phase.

Real integration tests cover safe assertions, violated assertions and
insufficient unwind bounds in both C and C++. They require an external reviewed
ESBMC installation; no ESBMC binaries are redistributed. The upstream prebuilt
bundle's own COPYING warns about mixed solver licenses; use a reviewed open-source
solver build before redistribution. The adapter explicitly selects Z3, but that
does not change the contents or licensing of a prebuilt bundle.
