// CHECKS: misra-c2012-rule-21-1
#undef __LINE__ // NONCOMPLIANT
#define _GUARD_H 1 // NONCOMPLIANT
#define errno my_errno // NONCOMPLIANT
#define isneg(x) ((x) < 0)
#define NDEBUG
#define PROJECT_LIMIT 4
