// CHECKS: misra-c2012-rule-13-6
#include <stddef.h>
volatile int i;
int j;
extern int f(void);
void g(int n) {
  size_t s;
  s = sizeof(j);
  s = sizeof(j++); // NONCOMPLIANT
  s = sizeof(i);
  s = sizeof(int);
  s = sizeof(f()); // NONCOMPLIANT
  s = sizeof(int[n]);
  s = sizeof(int[n++]); // NONCOMPLIANT
  (void)s;
}
