// CHECKS: misra-c2012-rule-20-8
#define FALSE 0
#define TRUE 1
#define LEVEL 2
#if FALSE
#endif
#if 10 // NONCOMPLIANT
#endif
#if !defined(X)
#endif
#if LEVEL > 1
#endif
#if LEVEL // NONCOMPLIANT
#endif
#if 0
#if 5
#endif
#endif
