// CHECKS: misra-c2012-rule-16-3
void f(int x, int a, int b) {
  switch (x) {
  case 0:
    break;
  case 1:
  case 2:
    break;
  case 4: // NONCOMPLIANT
    a = b;
  case 5: // NONCOMPLIANT
    if (a == b) {
      ++a;
      break;
    }
  default: // NONCOMPLIANT
    ;
  }
  switch (x) {
  case 0: {
    a = 1;
    break;
  }
  default:
    break;
  }
}
