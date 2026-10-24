// CHECKS: misra-c2012-rule-5-4
#define mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm_raw 1
#define mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm_scaled 2 // NONCOMPLIANT
#define SHORT_RAW 1
#define SHORT_SCALED 2
#define SAME(SAME) (SAME) // NONCOMPLIANT
#define PAIR(x, y) ((x) + (y))
