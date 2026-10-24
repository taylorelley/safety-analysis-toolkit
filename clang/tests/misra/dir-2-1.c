// CHECKS: clang-diagnostic-error,misra-c2012-dir-4-2
int missing(void) { return undeclared_name; } // NONCOMPLIANT
int fine(void) { return 0; }
