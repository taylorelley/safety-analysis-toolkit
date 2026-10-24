// CHECKS: misra-c2012-rule-9-4
short a1[5] = {-5, -4, -3, -2, -1};
short a2[5] = {[0] = -5, [1] = -4, [2] = -3, [3] = -2, [4] = -1};
short a3[5] = {[0] = -5, [1] = -4, [2] = -3, [2] = -2, [4] = -1}; // NONCOMPLIANT
struct mystruct { int a; int b; int c; int d; };
struct mystruct s1 = {100, -1, 42, 999};
struct mystruct s2 = {.a = 100, .b = -1, .c = 42, .d = 999};
struct mystruct s3 = {.a = 100, .b = -1, .a = 42, .d = 999}; // NONCOMPLIANT
short a4[3] = {[1] = 1, 2, [0] = 0};
