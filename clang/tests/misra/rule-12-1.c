// CHECKS: misra-c2012-rule-12-1
int f(int a, int b, int c, int x, int y, int *p) {
  int r;
  r = sizeof x + y; // NONCOMPLIANT
  r = a + b;
  r = a + b + c;
  r = (a + b) - (c + x);
  r = (a == b) ? a : (a - b);
  r = a == b ? a : a - b; // NONCOMPLIANT
  r = a << (b + c);
  r = (a && b && c);
  r = a + b * c; // NONCOMPLIANT
  r = *p + sizeof(x);
  return r;
}
