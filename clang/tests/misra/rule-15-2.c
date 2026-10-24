// CHECKS: misra-c2012-rule-15-2
void f(void) {
  int j = 0;
L1:
  ++j;
  if (10 == j) {
    goto L2;
  }
  goto L1; // NONCOMPLIANT
L2:
  ++j;
}
