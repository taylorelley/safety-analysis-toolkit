// CHECKS: misra-c2012-rule-2-5
#define SIZE 4
#define DATA 3 // NONCOMPLIANT
#define FLAG
#ifdef FLAG
#endif
int use(void) { return SIZE; }
