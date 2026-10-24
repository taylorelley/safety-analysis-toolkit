// CHECKS: misra-c2012-rule-20-6
// ARGS: -std=c99 -Wno-embedded-directive
#define M(A) (A)
const char *f(void) {
  return M(
#ifdef SW // NONCOMPLIANT
      "Message 1"
#else // NONCOMPLIANT
      "Message 2"
#endif // NONCOMPLIANT
  );
}
#ifdef SW
#endif
