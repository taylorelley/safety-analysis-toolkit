// CHECKS: misra-c2012-rule-12-3
extern void f(int a, int b);
void g(int *a) {
  int i;
  int *p;
  f((1, 2), 3); // NONCOMPLIANT
  for (i = 0, p = &a[0]; i < 4; ++i) { // NONCOMPLIANT
    (void)p;
  }
  f(1, 2);
}
