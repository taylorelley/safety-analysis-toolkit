// CHECKS: misra-c2012-rule-11-7
#include <stdint.h>
int16_t *p;
float f;
void g(void) {
  f = (float)(uintptr_t)p;
  _Bool b = (_Bool)p; // NONCOMPLIANT
  char c = (char)p; // NONCOMPLIANT
  p = (int16_t *)(intptr_t)7;
  (void)b; (void)c;
}
