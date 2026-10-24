// CHECKS: misra-c2012-rule-21-7
#include <stdlib.h>
int f(const char *s) {
  char *end;
  long v = strtol(s, &end, 10);
  return atoi(s) + (int)v; // NONCOMPLIANT
}
