// CHECKS: misra-c2012-rule-11-8
#include <stdint.h>
void f(void) {
  uint16_t x;
  uint16_t *const cpi = &x;
  uint16_t *const *pcpi = 0;
  uint16_t **ppi;
  const uint16_t *pci = 0;
  volatile uint16_t *pvi = 0;
  uint16_t *pi;
  pi = cpi;
  pi = (uint16_t *)pci; // NONCOMPLIANT
  pi = (uint16_t *)pvi; // NONCOMPLIANT
  ppi = (uint16_t **)pcpi; // NONCOMPLIANT
  pci = (const uint16_t *)pi;
  (void)ppi;
}
