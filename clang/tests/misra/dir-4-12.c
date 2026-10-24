// CHECKS: misra-c2012-dir-4-12
#include <stdlib.h>
void f(void) {
  char *p = malloc(4); // NONCOMPLIANT
  free(p); // NONCOMPLIANT
}
static char pool[4];
char *g(void) { return pool; }
