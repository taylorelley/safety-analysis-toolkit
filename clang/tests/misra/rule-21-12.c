// CHECKS: misra-c2012-rule-21-12
#include <fenv.h>
float f(float x, float y) {
  float z;
  (void)feclearexcept(FE_DIVBYZERO); // NONCOMPLIANT
  z = x / y;
  if (fetestexcept(FE_DIVBYZERO) != 0) { // NONCOMPLIANT
    z = 0.0f;
  }
  (void)fegetround();
  return z;
}
