// CHECKS: misra-c2012-rule-21-4
#include <setjmp.h> // NONCOMPLIANT
static jmp_buf env; // NONCOMPLIANT
void f(void) {
  if (setjmp(env) == 0) { // NONCOMPLIANT
    longjmp(env, 1); // NONCOMPLIANT
  }
}
