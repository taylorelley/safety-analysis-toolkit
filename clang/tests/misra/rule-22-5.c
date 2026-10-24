// CHECKS: misra-c2012-rule-22-5
#include <stdio.h>
FILE *pf1;
FILE *pf2;
void f(void) {
  FILE f3;
  pf2 = pf1;
  f3 = *pf2; // NONCOMPLIANT
  (void)f3;
  pf1->_flags = 0; // NONCOMPLIANT
}
