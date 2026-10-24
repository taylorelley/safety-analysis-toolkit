// CHECKS: misra-c2012-rule-20-14,clang-diagnostic-error
#ifdef A
#endif
#if 1 // NONCOMPLIANT
#include "rule-20-14_open.h"
