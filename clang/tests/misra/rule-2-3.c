// CHECKS: misra-c2012-rule-2-3
typedef int used_t;
typedef int unused_t; // NONCOMPLIANT
used_t f(void) {
  typedef short local_t; // NONCOMPLIANT
  return 67;
}
