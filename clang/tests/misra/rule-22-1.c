// CHECKS: clang-analyzer-unix.Malloc,clang-analyzer-unix.Stream
#include <stdio.h>
#include <stdlib.h>
int f(void) {
  void *b = malloc(40);
  return b != NULL; // NONCOMPLIANT
}
int g(void) {
  FILE *fp = fopen("tmp", "r");
  return fp != NULL; // NONCOMPLIANT
}
void h(void) {
  void *b = malloc(40);
  free(b);
}
