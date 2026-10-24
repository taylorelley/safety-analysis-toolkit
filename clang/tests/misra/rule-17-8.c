// CHECKS: misra-c2012-rule-17-8
short glob = 0;
void proc(short para) {
  para = glob; // NONCOMPLIANT
}
void f(char *p, char *q) {
  p = q; // NONCOMPLIANT
  *p = *q;
}
void g(int n) {
  n++; // NONCOMPLIANT
  int m = n;
  m++;
}
