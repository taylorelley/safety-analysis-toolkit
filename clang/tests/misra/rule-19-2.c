// CHECKS: misra-c2012-rule-19-2
#include <stdint.h>
uint32_t zext(uint16_t s) {
  union { // NONCOMPLIANT
    uint32_t ul;
    uint16_t us;
  } tmp;
  tmp.us = s;
  return tmp.ul;
}
struct plain { uint32_t v; };
