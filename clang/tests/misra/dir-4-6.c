// CHECKS: misra-c2012-dir-4-6
#include <stdint.h>
typedef int speed_type; // NONCOMPLIANT
typedef int SINT_16;
typedef int16_t torque_t;
int counter; // NONCOMPLIANT
int16_t good;
struct flags {
  unsigned int ready : 1;
  float ratio; // NONCOMPLIANT
};
long scale(int16_t v) { return v; } // NONCOMPLIANT
void take(short s); // NONCOMPLIANT
int main(int argc, char *argv[]) { (void)argc; (void)argv; return 0; }
