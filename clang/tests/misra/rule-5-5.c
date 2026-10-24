// CHECKS: misra-c2012-rule-5-5
#define Sum(x, y) ((x) + (y))
short Sum; // NONCOMPLIANT
#define LIMIT 4
short use(void) { return Sum(1, 2) + LIMIT; }
