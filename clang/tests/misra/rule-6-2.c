// CHECKS: misra-c2012-rule-6-2
struct s {
  signed int one : 1; // NONCOMPLIANT
  unsigned int flag : 1;
  signed int : 1;
  signed int two : 2;
};
