// CHECKS: misra-c2012-rule-5-3
void fn1(void) {
  short i;
  {
    short i; // NONCOMPLIANT
    i = 3;
  }
  (void)i;
}
struct astruct { short m; };
extern void g(struct astruct *p);
short xyz = 0;
void fn2(struct astruct xyz) { // NONCOMPLIANT
  g(&xyz);
}
unsigned short speed;
void fn3(void) {
  typedef float speed; // NONCOMPLIANT
}
void fn4(int count) {
  for (int k = 0; k < count; k++) {
    int inner = k;
    (void)inner;
  }
}
