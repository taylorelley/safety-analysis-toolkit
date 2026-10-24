// CHECKS: misra-c2012-rule-16-7
void f(int x, int y, int z) {
  switch (x == 0) { // NONCOMPLIANT
  case 0:
    y = x;
    break;
  default:
    y = z;
    break;
  }
  switch (x) {
  case 0:
    y = 1;
    break;
  default:
    break;
  }
  (void)y;
}
