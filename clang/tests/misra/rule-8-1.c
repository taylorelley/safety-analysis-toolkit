/* CHECKS: misra-c2012-rule-8-1 */
/* ARGS: -std=c90 -Wno-implicit-int */
extern x; /* NONCOMPLIANT */
extern int y;
const z = 1; /* NONCOMPLIANT */
extern f(void); /* NONCOMPLIANT */
extern int g(void);
extern void h(char c, const k); /* NONCOMPLIANT */
signed w;
