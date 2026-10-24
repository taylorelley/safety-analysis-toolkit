// CHECKS: misra-c2012-rule-10-7
#include "essential.h"
void f(void) {
  uint32_t r;
  r = u32a * u16a + u16b;
  r = (u32a * u16a) + u16b;
  r = u32a * ((uint32_t)u16a + u16b);
  u32a += (u32b + u16b);
  r = u32a * (u16a + u16b); // NONCOMPLIANT
  u32a += (u16a + u16b); // NONCOMPLIANT
  (void)r;
}
