// CHECKS: misra-c2012-rule-8-10
inline int twice(int x) { return x * 2; } // NONCOMPLIANT
static inline int thrice(int x) { return x * 3; }
int use(int x) { return twice(x) + thrice(x); }
