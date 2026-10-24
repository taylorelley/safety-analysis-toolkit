// CHECKS: misra-c2012-rule-8-4
extern short count;
short count = 0;
extern unsigned short speed = 6000u; // NONCOMPLIANT
unsigned char pressure = 101u; // NONCOMPLIANT
extern void func1(void);
void func1(void) {}
void func4(void) {} // NONCOMPLIANT
static void func5(void) {}
int main(void) { func5(); return 0; }
