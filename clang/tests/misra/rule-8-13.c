// CHECKS: misra-c2012-rule-8-13
#include <string.h>
#include <stdint.h>
uint16_t f(uint16_t *p) { // NONCOMPLIANT
  return *p;
}
uint16_t g(const uint16_t *p) { return *p; }
char last_char(char *const s) { // NONCOMPLIANT
  return s[strlen(s) - 1u];
}
uint16_t first(uint16_t a[5]) { // NONCOMPLIANT
  return a[0];
}
void set(uint16_t *p) { *p = 0u; }
void bump(uint16_t a[5]) { a[1]++; }
struct s { int v; };
void write_member(struct s *p) { p->v = 1; }
int read_member(struct s *p) { return p->v; } // NONCOMPLIANT
void copy(char *dst, const char *src) { (void)strcpy(dst, src); }
void forward(uint16_t *p) { set(p); }
