// CHECKS: misra-c2012-rule-21-10
#include <time.h> // NONCOMPLIANT
long f(void) {
  time_t now = time(0); // NONCOMPLIANT
  return (long)now;
}
