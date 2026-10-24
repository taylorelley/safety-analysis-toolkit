// CHECKS: misra-c2012-rule-10-3
#include "essential.h"
uint8_t foo1(uint16_t x) {
  return x; // NONCOMPLIANT
}
void f(void) {
  uint8_t u8x = 0;
  bool_t flag = (bool_t)0;
  bool_t set = true;
  bool_t get = (u8b > u8c);
  uint8_t bad = 1.0f; // NONCOMPLIANT
  bool_t blc = 0; // NONCOMPLIANT
  ena = a1;
  s8a = K1;
  u8a = 2;
  u8a = 2 * 24;
  cha += 1;
  u8a = u8b + u8c + u8d;
  u8a = (uint8_t)s8a;
  u32a = u16a;
  u32a = 2U + 125U;
  use_uint16(u8a);
  use_uint16(u8a + u16b);
  cha = 7; // NONCOMPLIANT
  u8a = 'a'; // NONCOMPLIANT
  u8b = 1 - 2; // NONCOMPLIANT
  u8c += 'a'; // NONCOMPLIANT
  use_uint32(s32a); // NONCOMPLIANT
  s8a = K2; // NONCOMPLIANT
  u16a = u32a; // NONCOMPLIANT
  use_uint16(u32a); // NONCOMPLIANT
  uint16_t zeros[2] = {0};
  switch (u8a) {
  case 1U:
    break;
  case 'x': // NONCOMPLIANT
    break;
  default:
    break;
  }
  (void)u8x; (void)flag; (void)set; (void)get; (void)bad; (void)blc; (void)zeros;
}
