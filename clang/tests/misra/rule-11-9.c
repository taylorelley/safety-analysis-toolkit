// CHECKS: misra-c2012-rule-11-9
#include <stddef.h>
#define MY_NULL_1 0
#define MY_NULL_2 (void *)0
int *p1 = 0; // NONCOMPLIANT
int *p2 = (void *)0;
void f(void) {
  if (p1 == MY_NULL_1) { // NONCOMPLIANT
  }
  if (p2 == MY_NULL_2) {
  }
  p1 = NULL;
  p2 = (p1 != NULL) ? p1 : 0; // NONCOMPLIANT
}
