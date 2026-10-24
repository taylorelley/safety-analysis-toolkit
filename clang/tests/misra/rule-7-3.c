// CHECKS: misra-c2012-rule-7-3
const long a = 0L;
const long b = 0l; // NONCOMPLIANT
const unsigned long c = 0Lu;
const unsigned long d = 0lU; // NONCOMPLIANT
const unsigned long long e = 0ULL;
const unsigned long long f = 0Ull; // NONCOMPLIANT
const long double m = 1.2L;
const long double n = 2.4l; // NONCOMPLIANT
const unsigned int h = 0xFFu;
