// CHECKS: misra-c2012-rule-15-1
void f(int x) {
  if (x > 0) {
    goto out; // NONCOMPLIANT
  }
out:
  return;
}
