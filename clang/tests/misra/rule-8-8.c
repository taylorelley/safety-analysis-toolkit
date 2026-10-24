// CHECKS: misra-c2012-rule-8-8
static int x = 0;
extern int x; // NONCOMPLIANT
static int f(void);
int f(void) { return 1; } // NONCOMPLIANT
static int g(void);
static int g(void) { return f() + x; }
int use(void) { return g(); }
