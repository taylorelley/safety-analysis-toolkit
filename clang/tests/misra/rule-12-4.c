// CHECKS: misra-c2012-rule-12-4
#define BASE 4294967040u
#define DELAY 4000000000u
#define WIDTH 600000000u
extern _Bool b;
void f(unsigned int x) {
  switch (x) {
  case BASE + 0u:
    break;
  case BASE + 512u: // NONCOMPLIANT
    break;
  default:
    break;
  }
  unsigned int off = DELAY + WIDTH; // NONCOMPLIANT
  unsigned int ok = (0u == 0u) ? 0u : (0u - 1u);
  unsigned int bad = b ? 0u : (0u - 1u); // NONCOMPLIANT
  const unsigned int c = 0xffffffffu;
  unsigned int y = c + 1u;
  (void)off; (void)ok; (void)bad; (void)y;
}
