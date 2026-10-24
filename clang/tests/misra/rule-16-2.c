// CHECKS: misra-c2012-rule-16-2
void f(int x, int flag) {
  switch (x) {
  case 1:
    if (flag) {
    case 2: // NONCOMPLIANT
      x = 1;
    }
    break;
  default:
    break;
  }
}
