// CHECKS: misra-c2012-rule-11-1
#include <stddef.h>
#include <stdint.h>
typedef void (*fp16)(int16_t n);
typedef void (*fp32)(int32_t n);
typedef fp16 (*pfp16)(void);
extern void f(int16_t n);
extern pfp16 pfp1;
void g(void) {
  fp16 fp1 = NULL;
  fp32 fp2 = (fp32)fp1; // NONCOMPLIANT
  if (fp2 != NULL) {
  }
  fp16 fp3 = (fp16)0x8000; // NONCOMPLIANT
  (void)(*pfp1());
  f(1);
  fp16 fp5 = f;
  void *v = (void *)f; // NONCOMPLIANT
  (void)fp3; (void)fp5; (void)v;
}
