// CHECKS: misra-c2012-rule-2-2
extern volatile unsigned short v;
extern char *p;
void f(void) {
  unsigned short x;
  (void)v;
  (int)v; // NONCOMPLIANT
  v >> 3; // NONCOMPLIANT
  x = 3;
  *p++; // NONCOMPLIANT
  (*p)++;
  x; // NONCOMPLIANT
  v;
}
