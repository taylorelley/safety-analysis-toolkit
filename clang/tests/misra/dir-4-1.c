// CHECKS: clang-analyzer-core.DivideZero,clang-analyzer-core.NullDereference
int divide(int a) {
  int z = 0;
  return a / z; // NONCOMPLIANT
}
int load(void) {
  int *p = 0;
  return *p; // NONCOMPLIANT
}
int safe(int a, int b) { return (b != 0) ? (a / b) : 0; }
