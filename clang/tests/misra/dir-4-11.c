// CHECKS: clang-analyzer-unix.StdCLibraryFunctions,clang-analyzer-core.NonNullParamChecker
#include <ctype.h>
#include <string.h>
int classify(void) {
  return toupper(1000); // NONCOMPLIANT
}
unsigned long length(void) {
  const char *s = 0;
  return strlen(s); // NONCOMPLIANT
}
int checked(int c) { return ((c >= 0) && (c <= 127)) ? toupper(c) : 0; }
