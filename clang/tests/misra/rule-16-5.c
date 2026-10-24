// CHECKS: misra-c2012-rule-16-5
void f(int x) {
  switch (x) {
  default:
  case 0:
    ++x;
    break;
  case 1:
  case 2:
    break;
  }
  switch (x) {
  case 0:
    ++x;
    break;
  default: // NONCOMPLIANT
    x = 0;
    break;
  case 1:
  case 2:
    break;
  }
}
