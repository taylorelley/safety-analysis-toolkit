// CHECKS: clang-analyzer-unix.Malloc
#include <stdlib.h>
void fn(void) {
  int a;
  free(&a); // NONCOMPLIANT
}
void g(void) {
  char *p = (char *)malloc(512);
  char *q = p;
  free(p);
  free(q); // NONCOMPLIANT
}
