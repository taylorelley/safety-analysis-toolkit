// CHECKS: clang-diagnostic-error,misra-c2012-dir-4-2
// Unknown types leave error-recovery declarations without source locations,
// which no check may report on (regression: Dir 4.6 exported a diagnostic
// with an empty file path on SQLite's pragma.c without its generated header).
static const Missing *locate(const char *z) { (void)z; return 0; } // NONCOMPLIANT
static Missing table[] = { 0 }; // NONCOMPLIANT
int use(void) { return locate("x") != 0 && table[0] != 0; }
