// CHECKS: misra-c2012-rule-21-2
#include <stddef.h>
extern void *memcpy(void *s1, const void *s2, size_t n); // NONCOMPLIANT
int _hidden; // NONCOMPLIANT
int visible;
static int helper(int value) { return value; }
int use(void) { return helper(visible); }
