// CHECKS: misra-c2012-rule-16-4
void f(int x, int errorflag) {
  switch (x) { // NONCOMPLIANT
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
  default:
    errorflag = 1;
    break;
  }
  switch (x) {
  case 0:
    break;
  default: break; // NONCOMPLIANT
  }
  switch (x) {
  case 0:
    break;
  default:
    /* nothing further is required */
    break;
  }
  (void)errorflag;
}
