// CHECKS: misra-c2012-rule-16-6
void f(int x, int y, int z) {
  switch (x) { // NONCOMPLIANT
  default:
    x = 0;
    break;
  }
  switch (y) { // NONCOMPLIANT
  case 1:
  default:
    y = 0;
    break;
  }
  switch (z) {
  case 1:
    z = 2;
    break;
  default:
    z = 0;
    break;
  }
}
