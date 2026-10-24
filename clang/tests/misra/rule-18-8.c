// CHECKS: misra-c2012-rule-18-8
#include <stdint.h>
void f(int16_t n) {
  uint16_t vla[n]; // NONCOMPLIANT
  uint16_t fixed[4];
  (void)vla; (void)fixed;
}
void h(uint16_t n, uint16_t a[10][n]) { // NONCOMPLIANT
  (void)a;
}
