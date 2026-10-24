// CHECKS: clang-analyzer-core.BitwiseShift
unsigned int f(unsigned int v) {
  int amount = 40;
  return v << amount; // NONCOMPLIANT
}
unsigned int g(unsigned int v) { return v << 3; }
