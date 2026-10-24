// CHECKS: misra-c2012-rule-20-9
#if M == 0 // NONCOMPLIANT
#endif
#if defined(M)
#if M == 0
#endif
#endif
#if defined(B) && (B == 0)
#endif
#define N 1
#if N
#endif
