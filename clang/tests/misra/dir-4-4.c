// CHECKS: misra-c2012-dir-4-4
/* This comment explains the purpose of the function. */
int value(int x) {
  /* x = x + 1; */ // NONCOMPLIANT
  /* NONCOMPLIANT */ // return 0;
  return x; // returns the input unchanged
}
