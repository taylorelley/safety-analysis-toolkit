// CHECKS: misra-c2012-rule-20-12
#define AA 0xffff
#define BB(x) (x) + wow##x
#define SCALE(X) ((X) * X##_scale)
int f(void) {
  int wowAA = 0;
  int speed = 2;
  int speed_scale = 3;
  wowAA = BB(AA); // NONCOMPLIANT
  return wowAA + SCALE(speed);
}
