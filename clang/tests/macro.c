#define TRANSFER(label) goto label
int macro_case(int x) { if (x) TRANSFER(done); x = 2; done: return x; }
