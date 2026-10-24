// CHECKS: misra-c2012-rule-17-4
// ARGS: -std=c99 -Wno-return-type
#include <stdint.h>
extern uint16_t table[10];
int32_t absolute(int32_t v) {
  if (v < 0) {
    return v;
  }
} // NONCOMPLIANT
uint16_t lookup(uint16_t v) {
  if (v > 9u) {
    return; // NONCOMPLIANT
  }
  return table[v];
}
int32_t sign(int32_t v) {
  if (v < 0) {
    return -1;
  } else {
    return 1;
  }
}
