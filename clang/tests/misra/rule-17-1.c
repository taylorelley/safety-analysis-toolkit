// CHECKS: misra-c2012-rule-17-1
#include <stdarg.h>
#include <stdint.h>
void h(va_list ap) { // NONCOMPLIANT
  double y;
  y = va_arg(ap, double); // NONCOMPLIANT
  (void)y;
}
void f(uint16_t n, ...) {
  uint32_t x;
  va_list ap; // NONCOMPLIANT
  va_start(ap, n); // NONCOMPLIANT
  x = va_arg(ap, uint32_t); // NONCOMPLIANT
  va_end(ap); // NONCOMPLIANT
  (void)x;
}
