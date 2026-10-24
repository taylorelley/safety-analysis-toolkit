// CHECKS: misra-c2012-rule-13-5
extern unsigned short h(unsigned short y);
extern unsigned short pure_fn(unsigned short y) __attribute__((const));
volatile unsigned short v;
unsigned short x;
void g(_Bool ishigh, unsigned short a) {
  if (ishigh && (a == pure_fn(x))) {
  }
  if (ishigh && (a == h(x))) { // NONCOMPLIANT
  }
  if ((x == 0u) || (v == 1u)) { // NONCOMPLIANT
  }
  if (ishigh && (x++ > 3u)) { // NONCOMPLIANT
  }
}
