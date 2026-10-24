// CHECKS: misra-c2012-dir-4-2
__asm__(".globl marker"); // NONCOMPLIANT
void f(void) { __asm__ volatile("nop"); } // NONCOMPLIANT
void g(void) { }
