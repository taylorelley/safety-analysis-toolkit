// CHECKS: misra-c2012-rule-14-4
#include <stdbool.h>
#include <stddef.h>
extern bool flag;
void f(int *p, int *q, int i) {
  while (p) { // NONCOMPLIANT
  }
  while (q != NULL) {
  }
  while (true) {
    break;
  }
  while (flag) {
  }
  if (i) { // NONCOMPLIANT
  }
  if (i != 0) {
  }
  for (;;) {
    break;
  }
}
