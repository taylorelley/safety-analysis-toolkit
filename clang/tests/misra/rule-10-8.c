// CHECKS: misra-c2012-rule-10-8
#include "essential.h"
void f(void) {
  uint32_t r;
  r = (uint16_t)(u32a + u32b);
  r = (uint16_t)(s32a + s32b); // NONCOMPLIANT
  r = (uint16_t)s32a;
  r = (uint32_t)(u16a + u16b); // NONCOMPLIANT
  r = (uint32_t)(2U + 3U);
  (void)r;
}
