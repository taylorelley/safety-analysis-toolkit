// CHECKS: misra-c2012-dir-4-9
#define DIV2(X) ((X) / 2) // NONCOMPLIANT
#define EVAL_BINOP(OP, L, R) ((L) OP (R))
#define SWAP(a, b) { int t = (a); (a) = (b); (b) = t; }
#define LEVEL 3
int f(int y) {
  int x = DIV2(y);
  int z = EVAL_BINOP(+, 1, 2);
  SWAP(x, z);
  return x + z + LEVEL;
}
