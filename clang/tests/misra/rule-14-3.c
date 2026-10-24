// CHECKS: misra-c2012-rule-14-3
#include <stdint.h>
#include <stdbool.h>
void f(uint16_t u16a, int8_t s8a) {
  int8_t r = (u16a < 0u) ? 0 : 1; // NONCOMPLIANT
  if (u16a <= 0xffffu) { // NONCOMPLIANT
  }
  if (2 > 3) { // NONCOMPLIANT
  }
  for (s8a = 0; s8a < 130; ++s8a) { // NONCOMPLIANT
  }
  while (true) {
    break;
  }
  do {
  } while (0u == 1u);
  if (u16a > 10u) {
  }
  (void)r;
}
