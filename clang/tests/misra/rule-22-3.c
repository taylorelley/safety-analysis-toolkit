// CHECKS: misra-c2012-rule-22-3
#include <stdio.h>
void fn(void) {
  FILE *fw = fopen("tmp", "r+");
  FILE *fr = fopen("tmp", "r"); // NONCOMPLIANT
  FILE *a = fopen("other", "r");
  FILE *b = fopen("other", "r");
  (void)fw; (void)fr; (void)a; (void)b;
}
