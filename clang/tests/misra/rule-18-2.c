// CHECKS: misra-c2012-rule-18-2
#include <stddef.h>
#include <stdint.h>
void f1(int32_t *ptr) {
  int32_t a1[10];
  int32_t a2[10];
  int32_t *p1 = &a1[1];
  int32_t *p2 = &a2[10];
  ptrdiff_t diff;
  diff = p1 - a1;
  diff = p2 - a2;
  diff = p1 - p2; // NONCOMPLIANT
  diff = ptr - p1; // NONCOMPLIANT
  diff = ptr - ptr;
  (void)diff;
}
