// CHECKS: misra-c2012-rule-9-2
short y1[3][2] = {1, 2, 0, 0, 5, 6}; // NONCOMPLIANT
short y2[3][2] = {{1, 2}, {0}, {5, 6}};
short z1[2][2] = {{0}, [1][1] = 1};
short z3[2][2] = {{0}, [1][0] = 0, 1}; // NONCOMPLIANT
short z4[2][2] = {[0][1] = 0, {0, 1}};
float a[3][2] = {0};
union u1 { short i; float f; } u = {0};
struct s1 { unsigned short len; char buf[8]; } s[3] = {
    {5u, {'a', 'b', 'c', 'd', 'e', '\0', '\0', '\0'}}, {2u, {0}}, {.len = 0u}};
char text[2][4] = {"abc", "def"};
