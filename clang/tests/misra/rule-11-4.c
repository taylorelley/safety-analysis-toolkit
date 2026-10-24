// CHECKS: misra-c2012-rule-11-4
#include <stdint.h>
#include <stddef.h>
uint8_t *PORTA = (uint8_t *)0x0002; // NONCOMPLIANT
uint16_t *p;
void f(void) {
  int64_t addr = (int64_t)&p; // NONCOMPLIANT
  uint8_t *q = (uint8_t *)addr; // NONCOMPLIANT
  _Bool b = (_Bool)p; // NONCOMPLIANT
  uint8_t *r = NULL;
  uint8_t *z = 0;
  (void)q; (void)b; (void)r; (void)z;
}
