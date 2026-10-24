// CHECKS: misra-c2012-rule-17-2
int even(int n);
int odd(int n) { return (n == 0) ? 0 : even(n - 1); } // NONCOMPLIANT
int even(int n) { return (n == 0) ? 1 : odd(n - 1); } // NONCOMPLIANT
int fact(int n) { return (n <= 1) ? 1 : (n * fact(n - 1)); } // NONCOMPLIANT
int square(int n) { return n * n; }
int use(int n) { return square(n); }
