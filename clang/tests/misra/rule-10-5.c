// CHECKS: misra-c2012-rule-10-5
#include "essential.h"
enum enumc { c1 } enc;
void f(void) {
  bool_t b = (bool_t) false;
  int32_t i = (int32_t)3U;
  b = (bool_t)0;
  b = (bool_t)3U; // NONCOMPLIANT
  i = (int32_t)ena;
  ena = (enum enuma)3; // NONCOMPLIANT
  cha = (char)enc;
  f32a = (float32_t)cha; // NONCOMPLIANT
  i = (int32_t)bla; // NONCOMPLIANT
  ena = (enum enuma)a2;
  (void)b; (void)i;
}
