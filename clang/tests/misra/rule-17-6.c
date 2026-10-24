// CHECKS: misra-c2012-rule-17-6
#include <stdint.h>
uint16_t total(uint16_t n, uint16_t a[static 20]) { // NONCOMPLIANT
  uint16_t sum = 0U;
  for (uint16_t i = 0U; i < n; ++i) {
    sum = sum + a[i];
  }
  return sum;
}
uint16_t first(const uint16_t a[20]) { return a[0]; }
