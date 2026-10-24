// CHECKS: misra-c2012-rule-18-7
#include <stdint.h>
struct s {
  uint16_t len;
  uint32_t data[]; // NONCOMPLIANT
};
struct t {
  uint16_t len;
  uint32_t data[4];
};
