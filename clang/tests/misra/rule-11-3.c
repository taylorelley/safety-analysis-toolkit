// CHECKS: misra-c2012-rule-11-3
#include <stdint.h>
uint8_t *p1;
uint32_t *p2;
void f(void) {
  p2 = (uint32_t *)p1; // NONCOMPLIANT
  uint32_t u = 0U;
  uint16_t *hi_p = (uint16_t *)&u; // NONCOMPLIANT
  unsigned char *bytes = (unsigned char *)&u;
  const short *p = 0;
  const volatile short *q = (const volatile short *)p;
  int *const *pcpi = 0;
  const int *const *pcpci = (const int *const *)pcpi; // NONCOMPLIANT
  (void)hi_p; (void)bytes; (void)q; (void)pcpci;
}
