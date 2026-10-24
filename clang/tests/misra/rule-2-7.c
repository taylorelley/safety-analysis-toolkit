// CHECKS: misra-c2012-rule-2-7
void withunused(unsigned short *para1, int unusedpara) { // NONCOMPLIANT
  *para1 = 42U;
}
void voided(int a) { (void)a; }
int decl(int b);
