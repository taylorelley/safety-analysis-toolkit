// CHECKS: misra-c2012-rule-15-7
extern void action(void);
void f(int flag_1, int flag_2, int flag_3) {
  if (flag_1) {
    action();
  } else if (flag_2) { // NONCOMPLIANT
    action();
  }
  if (flag_1) {
    action();
  } else if (flag_2) {
    action();
  } else {
    ; /* No action required */
  }
  if (flag_1) {
    action();
  } else if (flag_3) {
    action();
  } /* NONCOMPLIANT */ else {
  }
  if (flag_1) {
    action();
  }
}
