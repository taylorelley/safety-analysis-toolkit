// CHECKS: misra-c2012-rule-13-4
extern int f(void);
void g(int *a, int x, int y, int b, int c, _Bool bool_var) {
  x = y;
  a[x] = a[x = y]; // NONCOMPLIANT
  if (bool_var = 0) { // NONCOMPLIANT
  }
  if ((x = f()) != 0) { // NONCOMPLIANT
  }
  a[b += c] = a[b]; // NONCOMPLIANT
  x = b = c = 0; // NONCOMPLIANT
  for (x = 0; x < 3; x += 1) {
  }
}
