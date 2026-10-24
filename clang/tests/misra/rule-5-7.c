// CHECKS: misra-c2012-rule-5-7
struct deer { unsigned short a; };
void foo(void) {
  struct deer { // NONCOMPLIANT
    unsigned short a;
  };
}
typedef struct coord { unsigned short x; } coord;
struct stag { int a; };
struct stag a1;
int point; // distinct
