// CHECKS: clang-analyzer-unix.Stream,clang-analyzer-unix.Malloc
#include <stdio.h>
#include <stdlib.h>
void leak(void) {
  FILE *fp = fopen("tmp", "r");
  if (fp != NULL) {
    (void)fgetc(fp); // NONCOMPLIANT
  }
}
void twice(void) {
  char *p = malloc(4);
  free(p);
  free(p); // NONCOMPLIANT
}
void balanced(void) {
  FILE *fp = fopen("tmp", "r");
  if (fp != NULL) {
    (void)fclose(fp);
  }
}
