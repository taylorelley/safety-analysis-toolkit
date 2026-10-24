/* CHECKS: misra-c2012-dir-1-1 */
/* ARGS: -std=c90 */
int quotient(int a, int b, unsigned int u) {
  int q = a / b; /* NONCOMPLIANT */
  int r = a % b; /* NONCOMPLIANT */
  unsigned int v = u / 2U;
  return q + r + (int)v + (7 / 2);
}
