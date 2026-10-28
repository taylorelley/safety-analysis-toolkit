// CHECKS: misra-c2012-rule-9-5
int a1[] = {[0] = 1}; // NONCOMPLIANT
int a2[10] = {[0] = 1};
int a3[] = {1, 2, 3};
/* An extern declaration of an array defined later with an initializer: only
   the definition carries the initializer (regression: crashed on SQLite). */
extern const unsigned char t1[];
const unsigned char t1[] = {1, 2};
extern int t2[];
int t2[] = {[1] = 2}; // NONCOMPLIANT
