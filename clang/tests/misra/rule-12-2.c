// CHECKS: misra-c2012-rule-12-2
#include <stdint.h>
void f(uint8_t u8a, uint16_t u16a, uint32_t n) {
  u8a = (uint8_t)(u8a << 7);
  u8a = (uint8_t)(u8a << 8); // NONCOMPLIANT
  u16a = (uint16_t)((uint16_t)u8a << 9);
  u16a = (uint16_t)(1u << 10u); // NONCOMPLIANT
  u16a = (uint16_t)((uint16_t)1u << 10u);
  n = 1UL << 10u;
  n = n << n;
  n = n >> -1; // NONCOMPLIANT
  (void)u8a; (void)u16a;
}
