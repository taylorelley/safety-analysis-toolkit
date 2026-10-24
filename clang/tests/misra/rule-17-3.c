/* CHECKS: misra-c2012-rule-17-3 */
/* ARGS: -std=c90 -Wno-implicit-function-declaration */
extern double declared(double d, int n);
void func(void) {
  double sq1 = power(1, 2.0); /* NONCOMPLIANT */
  double sq2 = declared(1.0, 2);
  (void)sq1;
  (void)sq2;
}
