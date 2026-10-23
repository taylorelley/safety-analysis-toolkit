/* Intentionally unsafe analyzer fixture; do not deploy. */
int unsafe_value(void) { int *p = 0; return *p; }
