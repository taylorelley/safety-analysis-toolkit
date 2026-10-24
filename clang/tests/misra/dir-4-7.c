// CHECKS: misra-c2012-dir-4-7
#include <stdio.h>
void f(FILE *fp) {
  fclose(fp); // NONCOMPLIANT
  (void)fflush(fp); // NONCOMPLIANT
  if (fputs("x", fp) == EOF) {
  }
  int r = remove("x");
  (void)r;
}
