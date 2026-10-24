// CHECKS: misra-c2012-rule-20-1
#define F1_MACRO
#include "rule-20-1_a.h"
/* a comment */
#include <stddef.h>
int i = 0;
#include <stdint.h> // NONCOMPLIANT
