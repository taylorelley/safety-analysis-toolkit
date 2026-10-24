// CHECKS: misra-c2012-rule-5-8
int count;
void foo(void) {
  short index;
  (void)index;
}
void bar(void) {
  short count; // NONCOMPLIANT
  short index;
  (void)count;
  (void)index;
}
