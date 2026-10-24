// CHECKS: misra-c2012-rule-15-6
extern int data_available;
extern void process_data(void);
void f(int flag_1, int flag_2) {
  while (data_available)
    process_data(); // NONCOMPLIANT
  if (flag_1) {
    process_data();
  } else if (flag_2) {
    process_data();
  } else {
  }
  while (flag_1); // NONCOMPLIANT
  for (;;) {
    break;
  }
}
