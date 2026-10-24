// CHECKS: misra-c2012-rule-6-1
typedef unsigned int UINT_16;
typedef signed int SINT_16;
struct s {
  unsigned int b1 : 2;
  int b2 : 2; // NONCOMPLIANT
  UINT_16 b3 : 2;
  signed long b4 : 2; // NONCOMPLIANT
  signed int b5 : 2;
  SINT_16 b6 : 2;
  _Bool b7 : 1;
  unsigned char b8 : 2; // NONCOMPLIANT
};
