// CHECKS: misra-c2012-rule-19-1
#include <string.h>
#include <stdint.h>
int16_t a[20];
void fn(void) {
  union {
    int16_t i;
    int32_t j;
  } u = {0}, v = {1};
  u.j = u.i; // NONCOMPLIANT
  u = v;
}
void f(void) {
  (void)memcpy(&a[5], &a[4], 2u * sizeof(a[0])); // NONCOMPLIANT
  (void)memcpy(&a[10], &a[0], 4u * sizeof(a[0]));
  (void)memmove(&a[5], &a[4], 2u * sizeof(a[0]));
}
void g(void) {
  int16_t *p = &a[0];
  int16_t *q = &a[0];
  *p = *q;
}
