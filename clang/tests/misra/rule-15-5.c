// CHECKS: misra-c2012-rule-15-5
#include <stdbool.h>
#include <stddef.h>
bool f(unsigned short n, char *p) {
  if (n > 10u) {
    return false; // NONCOMPLIANT
  }
  if (p == NULL) {
    return false; // NONCOMPLIANT
  }
  return true;
}
int g(int x) {
  int r = x;
  return r;
}
