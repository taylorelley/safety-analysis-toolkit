// CHECKS: misra-c2012-rule-10-2
#include "essential.h"
void f(void) {
  int32_t r;
  r = '0' + u8a;
  r = s8a + '0';
  r = cha - '0';
  r = '0' - s8a;
  r = s16a - 'a'; // NONCOMPLIANT
  r = (int32_t)('0' + f32a); // NONCOMPLIANT
  r = cha + ':'; // NONCOMPLIANT
  r = cha - ena; // NONCOMPLIANT
  (void)r;
}
