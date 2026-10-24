// CHECKS: clang-analyzer-core.DivideZero,clang-analyzer-core.uninitialized.Assign
int f(void) {
  int u;
  int v = u; // NONCOMPLIANT
  return v;
}
int g(int a) {
  int z = 0;
  return a / z; // NONCOMPLIANT
}
int h(int a) { return a + 1; }
