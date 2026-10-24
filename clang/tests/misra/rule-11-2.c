// CHECKS: misra-c2012-rule-11-2
#include <stddef.h>
struct s;
struct t;
struct s *sp;
struct t *tp;
short *ip;
struct s *f(void);
void g(void) {
  ip = (short *)sp; // NONCOMPLIANT
  sp = (struct s *)1234; // NONCOMPLIANT
  tp = (struct t *)sp; // NONCOMPLIANT
  sp = NULL;
  (void)f();
  struct s *same = sp;
  (void)same;
}
