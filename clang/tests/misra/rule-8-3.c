// CHECKS: misra-c2012-rule-8-3
extern void f(signed int);
void f(int a) { (void)a; }
extern void g(int *const p);
void g(int *p) { (void)p; } // NONCOMPLIANT
extern short func(short num, short den);
short func(short den, short num) { return num / den; } // NONCOMPLIANT
typedef unsigned short width_t;
typedef unsigned short height_t;
extern unsigned int area(width_t w, height_t h);
unsigned int area(width_t w, width_t h) { return (unsigned int)w * h; } // NONCOMPLIANT
extern int counter;
int counter = 0;
