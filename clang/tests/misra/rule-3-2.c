// CHECKS: misra-c2012-rule-3-2
void f(int *b) {
  int x = 0; // comment NONCOMPLIANT \
  x = 5;
  *b = x; // ordinary comment
}
