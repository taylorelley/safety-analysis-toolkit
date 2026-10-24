// CHECKS: misra-c2012-rule-3-1
/* some comment /* nested start */ // NONCOMPLIANT
/* see http://example.com */ // NONCOMPLIANT
// a line comment with // inside is allowed
// a line comment with /* inside is not NONCOMPLIANT
/* a plain comment */
int x;
