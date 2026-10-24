// CHECKS: misra-c2012-rule-4-1
const char *s1 = "\x41g"; // NONCOMPLIANT
const char *s2 = "\x41" "g";
const char *s3 = "\x41\x67";
int c1 = '\141t'; // NONCOMPLIANT
int c2 = '\141\t';
const char *s4 = "\0";
const char *s5 = "\101" "\n";
