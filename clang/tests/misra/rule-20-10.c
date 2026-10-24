// CHECKS: misra-c2012-rule-20-10
#define STR(x) #x // NONCOMPLIANT
#define CAT(x, y) x##y // NONCOMPLIANT
#define PLAIN(x) ((x) + 1)
