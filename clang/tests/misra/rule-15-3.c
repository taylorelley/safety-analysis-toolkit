// CHECKS: misra-c2012-rule-15-3
void f1(int a) {
  if (a <= 0) {
    goto L2; // NONCOMPLIANT
  }
  goto L1;
  if (a == 0) {
    goto L1;
  }
  goto L2; // NONCOMPLIANT
L1:
  if (a > 0) {
  L2:;
  }
}
void f2(int x, int y) {
  switch (x) {
  case 0:
    if (x == y) {
      goto L1; // NONCOMPLIANT
    }
    break;
  case 1:
    y = x;
  L1:
    ++x;
    break;
  default:
    break;
  }
}
