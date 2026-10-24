// CHECKS: misra-c2012-dir-1-1
#pragma pack(1) // NONCOMPLIANT
int shift(int s, unsigned int u) {
  int a = s >> 1; // NONCOMPLIANT
  unsigned int b = u >> 1;
  int c = 16 >> 2;
  return a + (int)b + c + (s / 2);
}
