// CHECKS: misra-c2012-rule-20-4
// ARGS: -std=c99 -Wno-keyword-macro
#define while(E) for (; (E);) // NONCOMPLIANT
#define unless(E) if (!(E))
#define inline // NONCOMPLIANT
