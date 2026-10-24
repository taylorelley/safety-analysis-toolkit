// CHECKS: clang-analyzer-core.StackAddressEscape
int *global;
void store(void) {
  int local = 0;
  global = &local;
} // NONCOMPLIANT
void fine(void) {
  static int kept = 0;
  global = &kept;
}
