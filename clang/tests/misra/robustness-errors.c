// CHECKS: clang-diagnostic-error,misra-c2012-dir-4-2
// Erroneous input must not crash any check; all MISRA checks are run on it.
struct incomplete;
int f(int a) {
  int x = a + missing_value; // NONCOMPLIANT
  switch (missing_value) { // NONCOMPLIANT
  case 1:
    break;
  }
  if (missing_call(a) == 0u) {
  }
  struct incomplete s; // NONCOMPLIANT
  return x + (int)sizeof(missing_type); // NONCOMPLIANT
}
