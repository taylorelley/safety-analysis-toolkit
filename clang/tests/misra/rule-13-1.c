// CHECKS: misra-c2012-rule-13-1
volatile unsigned short v1;
unsigned short x = 0u;
extern void p(unsigned short a[2]);
void f(void) {
  unsigned short a[2] = {v1, 0}; // NONCOMPLIANT
  (void)a;
}
void g(unsigned short y, unsigned short z) {
  unsigned short a[2] = {y + z, y - z};
  (void)a;
}
void h(void) {
  p((unsigned short[2]){x++, 0}); // NONCOMPLIANT
}
