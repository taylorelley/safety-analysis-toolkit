// CHECKS: misra-c2012-rule-8-7
#include "rule-8-7_api.h"
int shared = 1;
int local_only = 2; // NONCOMPLIANT
void exported(void) {}
void helper(void) {} // NONCOMPLIANT
static void hidden(void) {}
int main(void) { hidden(); helper(); return local_only; }
