// CHECKS: clang-analyzer-deadcode.DeadStores
int f(int a) {
  int x;
  x = a; // NONCOMPLIANT
  return a;
}
