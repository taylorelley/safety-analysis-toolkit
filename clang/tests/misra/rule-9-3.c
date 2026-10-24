// CHECKS: misra-c2012-rule-9-3
int x[3] = {0, 1, 2};
int y[3] = {0, 1}; // NONCOMPLIANT
float t[4] = {[1] = 1.0f, 2.0f}; // NONCOMPLIANT
float z[50] = {[1] = 1.0f, [25] = 2.0f};
float arr[3][2] = {{0.0f, 0.0f}, {1.0f, -1.0f}, {0}};
char h[10] = "Hello";
int zero[8] = {0};
