// CHECKS: misra-c2012-rule-20-11
#define A(x) #x
#define B(x, y) x##y
#define C(x, y) #x##y // NONCOMPLIANT
