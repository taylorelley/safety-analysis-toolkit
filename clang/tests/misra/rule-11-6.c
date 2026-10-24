// CHECKS: misra-c2012-rule-11-6
#include <stdint.h>
void *p;
uint32_t u;
void f(void) {
  p = (void *)0x1234u; // NONCOMPLIANT
  p = (void *)p;
  u = (uint32_t)p; // NONCOMPLIANT
  p = (void *)0;
}
