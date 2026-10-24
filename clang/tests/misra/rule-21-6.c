// CHECKS: misra-c2012-rule-21-6
#include <stdio.h>
#include <string.h>
void f(const char *s) {
  (void)printf("%s", s); // NONCOMPLIANT
  FILE *fp = fopen("x", "r"); // NONCOMPLIANT
  (void)fp;
  (void)strlen(s);
}
