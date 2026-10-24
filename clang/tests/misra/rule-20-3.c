// CHECKS: misra-c2012-rule-20-3
// ARGS: -std=c99 -Wno-extra-tokens
#include "essential.h"
#define HEADER "essential.h"
#include HEADER
#include <stddef.h> extra // NONCOMPLIANT
