// CHECKS: misra-c2012-rule-18-5
#include <stdint.h>
typedef int8_t *INTPTR;
void function(int8_t **arrPar[]) { // NONCOMPLIANT
  int8_t **obj2;
  int8_t ***obj3; // NONCOMPLIANT
  INTPTR *obj4;
  INTPTR *const *const obj5 = 0; // NONCOMPLIANT
  int8_t **arr[10];
  int8_t **(*parr)[10];
  int8_t *(**pparr)[10];
  (void)obj2; (void)obj3; (void)obj4; (void)obj5; (void)arr; (void)parr; (void)pparr;
}
struct s {
  int8_t *s1;
  int8_t **s2;
  int8_t ***s3; // NONCOMPLIANT
};
struct s *ps1;
struct s **ps2;
struct s ***ps3; // NONCOMPLIANT
int8_t **(*pfunc1)(void);
int8_t **(**pfunc2)(void);
int8_t **(***pfunc3)(void); // NONCOMPLIANT
int8_t ***(**pfunc4)(void); // NONCOMPLIANT
