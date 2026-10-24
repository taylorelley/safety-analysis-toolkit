// CHECKS: misra-c2012-rule-2-1
int f(int c) {
  int r = c;
  switch (c) {
  case 0:
    r = 1;
    break;
  default:
    break;
  }
  return r;
  r = c; // NONCOMPLIANT
}
int g(int c) {
  while (1) {
    if (c > 3) {
      break;
    }
    c++;
  }
  return c;
}
void h(void) {
  for (;;) {
  }
  h(); // NONCOMPLIANT
}
