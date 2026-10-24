// CHECKS: misra-c2012-rule-14-2
extern _Bool C(void);
extern int g(void);
void f(int *a, int n) {
  _Bool flag = 0;
  for (int i = 0; (i < 5) && !flag; i++) {
    if (C()) {
      flag = 1;
    }
    i = i + 3; // NONCOMPLIANT
  }
  for (;;) {
    break;
  }
  for (int j = 0; j < n; j++, a++) { // NONCOMPLIANT
  }
  int k;
  for (k = 0; k < g(); k++) { // NONCOMPLIANT
  }
  for (k = 0; k < n; k++) {
    a[k] = 0;
  }
  int limit = n;
  for (k = 0; k < limit; k++) { // NONCOMPLIANT
    limit--;
  }
}
