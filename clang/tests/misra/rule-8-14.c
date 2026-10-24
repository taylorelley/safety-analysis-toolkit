// CHECKS: misra-c2012-rule-8-14
#include <string.h>
void user_copy(void *restrict p, void *restrict q, unsigned long n); // NONCOMPLIANT
void f(char *d, const char *s) { (void)memcpy(d, s, 1u); }
