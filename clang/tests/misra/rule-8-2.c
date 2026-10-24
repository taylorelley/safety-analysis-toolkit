// CHECKS: misra-c2012-rule-8-2
extern short func1(short n);
extern void func2(short); // NONCOMPLIANT
static short func3(); // NONCOMPLIANT
static short func4(void);
short func1(short n) { return n; }
static short func3(vec, n) // NONCOMPLIANT
short *vec;
short n;
{
  return vec[n - 1];
}
short (*pf1)(); // NONCOMPLIANT
short (*pf2)(void);
typedef short (*pf2_t)(short); // NONCOMPLIANT
typedef short (*pf3_t)(short n);
static short func4(void) { return func3((short *)0, 1); }
