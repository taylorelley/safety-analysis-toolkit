// CHECKS: misra-c2012-dir-4-8
#include "dir-4-8_opaque.h"
struct hidden *handle(struct hidden *h) { return h; }
int read_used(struct used *u) { return u->value; }
