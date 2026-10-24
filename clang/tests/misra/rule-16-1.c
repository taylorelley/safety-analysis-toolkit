// CHECKS: misra-c2012-rule-16-1
void f(int x, int y) {
  switch (x) { // NONCOMPLIANT
  case 0:
    y = 1;
  default:
    break;
  }
  switch (x) {
  case 0:
    y = 2;
    break;
  default:
    y = 3;
    break;
  }
  switch (x) { // NONCOMPLIANT
  case 0:
    break;
  case 1:
    break;
  }
  (void)y;
}
