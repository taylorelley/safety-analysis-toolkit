// CHECKS: misra-c2012-rule-5-6
void func(void) {
  {
    typedef unsigned char u8_t;
  }
  {
    typedef unsigned char u8_t; // NONCOMPLIANT
  }
}
typedef float mass;
void func1(void) {
  float mass = 0.0f; // NONCOMPLIANT
  (void)mass;
}
typedef struct list {
  struct list *next;
} list;
typedef struct {
  struct chain { // NONCOMPLIANT
    struct chain *list; // NONCOMPLIANT
  } s1;
} chain;
