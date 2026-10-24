// CHECKS: misra-c2012-rule-10-1
#include "essential.h"
void f(int32_t *arr) {
  (void)(cha && bla); // NONCOMPLIANT
  (void)(ena ? a1 : a2); // NONCOMPLIANT
  (void)(s8a && bla); // NONCOMPLIANT
  (void)(bla * blb); // NONCOMPLIANT
  (void)(bla > blb); // NONCOMPLIANT
  (void)(cha & chb); // NONCOMPLIANT
  (void)(cha << 1); // NONCOMPLIANT
  ena--; // NONCOMPLIANT
  (void)(ena * a1); // NONCOMPLIANT
  (void)(s8a & 2); // NONCOMPLIANT
  (void)(50 << 3U); // NONCOMPLIANT
  (void)(u8a << s8a); // NONCOMPLIANT
  (void)(-u8a); // NONCOMPLIANT
  (void)(arr[bla]); // NONCOMPLIANT
  (void)(bla && blb);
  (void)(bla ? u8a : u8b);
  (void)(cha - chb);
  (void)(cha > chb);
  (void)(ena > a1);
  (void)(K1 * s8a);
  (void)(s8a + s16b);
  (void)(-(s8a) * s8b);
  (void)(s8a > 0);
  --s16b;
  (void)(u8a + u16b);
  (void)(u8a & 2U);
  (void)(u8a << 2U);
  (void)(u8a << 1);
  (void)(f32a + f32b);
  (void)(f32a > 0.0);
  (void)(arr[ena]);
}
