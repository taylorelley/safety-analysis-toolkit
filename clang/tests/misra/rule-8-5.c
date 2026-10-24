// CHECKS: misra-c2012-rule-8-5
#include "rule-8-5_feature.h"
extern short a; // NONCOMPLIANT
short a = 0;
void work(void) {}
