namespace control {
template<class T> T transform(T x) { if (x) goto end; x = T{}; end: return x; }
int transform(int x, int y) { return x + y; }
int instantiate() { return transform(1); }
}
