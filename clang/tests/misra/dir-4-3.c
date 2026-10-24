// CHECKS: misra-c2012-dir-4-3
#define NOP() __asm__ volatile("nop")
void isolated(void) { __asm__ volatile("nop"); }
void via_macro(int *p) { *p = 1; NOP(); }
void mixed(int *p) {
  *p = 1;
  __asm__ volatile("nop"); // NONCOMPLIANT
}
