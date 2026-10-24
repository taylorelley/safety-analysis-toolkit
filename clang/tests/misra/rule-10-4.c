// CHECKS: misra-c2012-rule-10-4
#include "essential.h"
void f(void) {
  (void)(ena > a1);
  (void)(u8a + u16b);
  cha += u8a;
  s8a += u8a; // NONCOMPLIANT
  (void)(u8b + 2); // NONCOMPLIANT
  (void)(enb > a1); // NONCOMPLIANT
  (void)(ena == enb); // NONCOMPLIANT
  u8a += cha; // NONCOMPLIANT
  (void)(bla ? u8a : s8a); // NONCOMPLIANT
  (void)(cha - 1);
}
