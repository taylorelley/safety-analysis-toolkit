// CHECKS: misra-c2012-rule-20-7
#define M1(x, y) (x * y)
#define M2(x, y) ((x) * (y))
#define M3(x) a##x = (x)
#define GET_MEMBER(S, M) (S).M
struct s { int minval; };
int f(struct s s1) {
  int r = M1(1 + 2, 3 + 4); // NONCOMPLIANT
  r = M1((1 + 2), (3 + 4));
  r = M2(1 + 2, 3 + 4);
  r = M1(r, 2);
  int M3(0);
  r = r + a0 + GET_MEMBER(s1, minval);
  return r;
}
