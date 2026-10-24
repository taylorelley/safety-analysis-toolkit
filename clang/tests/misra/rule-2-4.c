// CHECKS: misra-c2012-rule-2-4
void unusedtag(void) {
  enum state { S_init, S_run }; // NONCOMPLIANT
}
typedef struct record_t { // NONCOMPLIANT
  int key;
} record1_t;
typedef struct {
  int key;
} record2_t;
struct used { int v; };
struct list { struct list *next; };
int read(struct used *u, record1_t *r, record2_t *s, struct list *l) { return u->v + r->key + s->key + (l != 0); }
