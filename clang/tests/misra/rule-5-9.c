// CHECKS: misra-c2012-rule-5-9
static int count;
static void foo(void) {
  short count; // NONCOMPLIANT
  short index;
  (void)count;
  (void)index;
}
void bar1(void) {
  static short count; // NONCOMPLIANT
  short index;
  (void)count;
  (void)index;
  foo();
}
