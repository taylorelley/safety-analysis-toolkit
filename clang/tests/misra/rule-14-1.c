// CHECKS: misra-c2012-rule-14-1
#include <stdint.h>
float read_float(void);
uint32_t read_u32(void);
void f(void) {
  uint32_t counter = 0u;
  for (float g = 0.0f; g < 1.0f; g += 0.001f) { // NONCOMPLIANT
    ++counter;
  }
  float h;
  for (uint32_t c = 0u; c < 1000u; ++c) {
    h = (float)c * 0.001f;
  }
  float k = 0.0f;
  while (k < 1.0f) { // NONCOMPLIANT
    k += 0.001f;
  }
  float m = read_float();
  uint32_t u;
  do {
    u = read_u32();
  } while (((float)u - m) > 10.0f);
  (void)h;
}
