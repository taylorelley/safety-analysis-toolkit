// CHECKS: misra-c2012-rule-21-11
#include <tgmath.h> // NONCOMPLIANT
#include <math.h>
float f(float x) { return sqrtf(x); }
