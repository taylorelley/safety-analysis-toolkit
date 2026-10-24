// CHECKS: misra-c2012-rule-7-1
extern unsigned short code[10];
void f(void) {
  code[1] = 109;
  code[2] = 100;
  code[3] = 052; // NONCOMPLIANT
  code[4] = 071; // NONCOMPLIANT
  code[5] = 0;
  code[6] = 0x10;
}
