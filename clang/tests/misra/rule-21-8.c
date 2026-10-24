// CHECKS: misra-c2012-rule-21-8
#include <stdlib.h>
void f(int code) {
  if (getenv("HOME") == 0) { // NONCOMPLIANT
    exit(code); // NONCOMPLIANT
  }
  abort(); // NONCOMPLIANT
}
