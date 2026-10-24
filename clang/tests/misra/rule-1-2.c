// CHECKS: misra-c2012-rule-1-2
// ARGS: -std=gnu99
int f(int x) {
  int y = ({ int t = x; t; }); // NONCOMPLIANT
  int z = x ?: 1; // NONCOMPLIANT
  switch (x) {
  case 1 ... 3: // NONCOMPLIANT
    break;
  default:
    break;
  }
  __typeof__(x) w = 0; // NONCOMPLIANT
  return y + z + w;
}
int g(void) __attribute__((const)); // NONCOMPLIANT
int h(int a) { return a + 1; }
