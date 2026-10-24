// CHECKS: misra-c2012-rule-2-6
int f(int x) {
label1: // NONCOMPLIANT
  if (x > 0) {
    goto done;
  }
  x++;
done:
  return x;
}
