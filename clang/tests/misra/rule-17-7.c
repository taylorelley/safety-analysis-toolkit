// CHECKS: misra-c2012-rule-17-7
#include <stdint.h>
uint16_t func(uint16_t para1) { return para1; }
void none(void) {}
uint16_t x;
void discarded(uint16_t para2) {
  func(para2); // NONCOMPLIANT
  (void)func(para2);
  x = func(para2);
  none();
}
