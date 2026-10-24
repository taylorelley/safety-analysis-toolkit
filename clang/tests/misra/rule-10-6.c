// CHECKS: misra-c2012-rule-10-6
#include "essential.h"
void f(void) {
  u16c = u16a + u16b;
  u32a = (uint32_t)u16a + u16b;
  u32a = u16a + u16b; // NONCOMPLIANT
  use_uint32(u16a + u16b); // NONCOMPLIANT
  u32a = 2U + 3U;
}
