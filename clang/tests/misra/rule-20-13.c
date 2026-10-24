// CHECKS: misra-c2012-rule-20-13
#define AAA 2
int foo(void) {
  int x = 0;
#ifndef AAA
  x = 1;
#else1 // NONCOMPLIANT
  x = AAA;
#endif
  return x;
}
/*
#start is not a token in a comment
 */
#if 0
#define VALID 1
#warning not a C99 directive // NONCOMPLIANT
#endif
