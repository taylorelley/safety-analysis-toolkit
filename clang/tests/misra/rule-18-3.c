// CHECKS: misra-c2012-rule-18-3
#include <stdint.h>
struct limits { int32_t lwb; int32_t upb; };
void f1(void) {
  int32_t a1[10];
  int32_t a2[10];
  int32_t *p1 = a1;
  if (p1 < a1) {
  }
  if (p1 < a2) { // NONCOMPLIANT
  }
  struct limits limits_1 = {2, 5};
  struct limits limits_2 = {10, 5};
  if (&limits_1.lwb <= &limits_1.upb) {
  }
  if (&limits_1.lwb > &limits_2.upb) { // NONCOMPLIANT
  }
}
