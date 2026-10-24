// CHECKS: misra-c2012-rule-8-9
static unsigned int calls = 0; // NONCOMPLIANT
static unsigned int shared = 0;
unsigned int count(void) {
  ++calls;
  return calls + shared;
}
void reset(void) { shared = 0u; }
