// CHECKS: misra-c2012-rule-13-2
extern volatile unsigned short v1, v2;
extern volatile unsigned char PORT;
extern void f(unsigned short a, unsigned short b);
unsigned short t;
void g(unsigned short i, unsigned short *a, unsigned short x) {
  t = v1 + v2; // NONCOMPLIANT
  PORT = PORT & 0x80u;
  f(i++, i); // NONCOMPLIANT
  x = x = 0; // NONCOMPLIANT
  i = i++; // NONCOMPLIANT
  a[i] = i++; // NONCOMPLIANT
  x = x + 1u;
  x += 1u;
  if ((i > 0u) && (i++ < 10u)) {
  }
  (void)x;
}
