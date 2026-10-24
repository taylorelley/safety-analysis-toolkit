// CHECKS: misra-c2012-rule-21-5
#include <signal.h> // NONCOMPLIANT
void f(void) {
  (void)raise(SIGTERM); // NONCOMPLIANT
}
