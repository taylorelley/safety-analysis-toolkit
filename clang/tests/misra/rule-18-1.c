// CHECKS: misra-c2012-rule-18-1
#include <stdint.h>
void f2(void) {
  int32_t data = 0;
  int32_t c[10] = {0};
  int32_t d[5][2] = {0};
  int32_t *p1 = &c[0];
  int32_t *p2 = &c[10];
  int32_t *p3 = &c[11]; // NONCOMPLIANT
  c[-1] = 0; // NONCOMPLIANT
  data = c[10]; // NONCOMPLIANT
  d[3][1] = 0;
  data = d[2][3]; // NONCOMPLIANT
  p1 = c + 11; // NONCOMPLIANT
  p1 = c + 4;
  (void)p2; (void)p3; (void)data; (void)p1;
}
