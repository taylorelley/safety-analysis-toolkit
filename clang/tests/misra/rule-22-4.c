// CHECKS: misra-c2012-rule-22-4
#include <stdio.h>
void fn(void) {
  FILE *fp = fopen("tmp", "r");
  (void)fprintf(fp, "What happens now?"); // NONCOMPLIANT
  (void)fclose(fp);
  FILE *out = fopen("log", "w");
  (void)fputs("ok", out);
  (void)fclose(out);
}
