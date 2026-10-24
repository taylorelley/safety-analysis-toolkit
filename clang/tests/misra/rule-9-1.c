// CHECKS: clang-analyzer-core.UndefinedBinaryOperatorResult,clang-analyzer-core.uninitialized.Branch,clang-analyzer-core.uninitialized.Assign,clang-analyzer-core.CallAndMessage
void f(_Bool b, unsigned short *p) {
  if (b) {
    *p = 3U;
  }
}
int g(void) {
  unsigned short u;
  f(0, &u);
  if (u == 3U) { // NONCOMPLIANT
    return 1;
  }
  return 0;
}
int h(void) {
  unsigned short v = 2U;
  return v;
}
