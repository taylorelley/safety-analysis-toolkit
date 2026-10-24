// CHECKS: misra-c2012-rule-11-5
#include <stdint.h>
uint32_t *p32;
void *p;
uint16_t *p16;
void f(void) {
  p = p32;
  p16 = p; // NONCOMPLIANT
  p = (void *)p16;
  p32 = (uint32_t *)p; // NONCOMPLIANT
  p16 = (void *)0;
}
