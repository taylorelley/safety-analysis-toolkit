// CHECKS: misra-c2012-rule-7-4
char *s = "string"; // NONCOMPLIANT
const volatile char *p = "string";
extern void f1(char *s1);
extern void f2(const char *s2);
void g(void) {
  f1("string"); // NONCOMPLIANT
  f2("string");
  "0123456789"[0] = '*'; // NONCOMPLIANT
}
char *name1(void) {
  return ("MISRA"); // NONCOMPLIANT
}
const char *name2(void) {
  return ("MISRA");
}
char buffer[] = "array copy";
