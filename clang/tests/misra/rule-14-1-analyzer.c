// CHECKS: clang-analyzer-security.FloatLoopCounter
void f(void) {
  for (float x = 0.1f; x <= 1.0f; x += 0.1f) { // NONCOMPLIANT
  }
  for (int i = 0; i < 10; i++) {
  }
}
