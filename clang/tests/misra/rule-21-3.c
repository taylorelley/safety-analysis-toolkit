// CHECKS: misra-c2012-rule-21-3
#include <stdlib.h>
void f(void) {
  char *p = malloc(4); // NONCOMPLIANT
  free(p); // NONCOMPLIANT
  void *(*alloc)(size_t) = calloc != 0 ? malloc : 0; // NONCOMPLIANT
  (void)alloc;
}
int g(int x) { return abs(x); }
