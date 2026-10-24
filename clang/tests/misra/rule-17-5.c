// CHECKS: misra-c2012-rule-17-5
#include <stdint.h>
void fn1(int32_t array1[4]);
void fn2(int32_t array2[]);
void fn(int32_t *ptr) {
  int32_t arr3[3] = {1, 2, 3};
  int32_t arr4[4] = {0, 1, 2, 3};
  fn1(arr4);
  fn1(arr3); // NONCOMPLIANT
  fn1(ptr);
  fn2(arr4);
  fn2(ptr);
}
