// CHECKS: clang-analyzer-unix.Stream
#include <stdio.h>
void fn(void) {
  FILE *fp = fopen("tmp", "w");
  if (fp == NULL) {
    return;
  }
  (void)fclose(fp);
  (void)fprintf(fp, "?"); // NONCOMPLIANT
}
