// CHECKS: misra-c2012-rule-18-4
#include <stdint.h>
#include <stddef.h>
void fn1(uint8_t *p1) {
  uint8_t a[10];
  uint8_t *ptr;
  uint8_t index = 0U;
  index = index + 1U;
  a[index] = 0U;
  ptr = &a[5];
  ptr = a;
  ptr++;
  *(ptr + 5) = 0U; // NONCOMPLIANT
  ptr[5] = 0U;
  p1 = p1 + 5; // NONCOMPLIANT
  p1 -= 1; // NONCOMPLIANT
  ptrdiff_t d = ptr - p1;
  (void)d;
}
