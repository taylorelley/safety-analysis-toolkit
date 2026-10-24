// CHECKS: misra-c2012-rule-21-9
#include <stdlib.h>
static int cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
void f(int *v, size_t n) {
  qsort(v, n, sizeof(int), cmp); // NONCOMPLIANT
}
