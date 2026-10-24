// CHECKS: misra-c2012-rule-13-3
extern unsigned char f(void);
extern void g(unsigned char a);
void h(unsigned char u8a, unsigned char u8b, unsigned char u8c, unsigned char *p, unsigned char a[4]) {
  u8a = u8b++; // NONCOMPLIANT
  u8a = ++u8b + u8c--; // NONCOMPLIANT
  ++u8b;
  u8a = u8b + u8c;
  u8c--;
  a[u8a]++;
  *p++;
  (*p)++;
  if ((f() + --u8a) == 0u) { // NONCOMPLIANT
  }
  g(u8b++); // NONCOMPLIANT
  u8a = (1u == 1u) ? 0u : u8b++; // NONCOMPLIANT
}
