// CHECKS: misra-c2012-rule-18-6
#include <stdint.h>
int8_t *func(void) {
  int8_t local_auto = 0;
  return &local_auto; // NONCOMPLIANT
}
uint16_t *sp;
void h(void) {
  static uint16_t *q;
  uint16_t x = 0u;
  uint16_t *r;
  q = &x; // NONCOMPLIANT
  r = &x;
  (void)r;
}
static uint16_t keep;
uint16_t *ok(void) { return &keep; }
void out(uint16_t **pp) {
  uint16_t buf[4] = {0};
  *pp = buf; // NONCOMPLIANT
}
