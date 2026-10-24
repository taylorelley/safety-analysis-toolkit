// CHECKS: misra-c2012-rule-20-5
#define QUALIFIER volatile
#undef QUALIFIER // NONCOMPLIANT
int x;
