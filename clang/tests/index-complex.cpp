namespace flight {
/** @req R-CLASS */
struct Controller { int step(int x) { return x+1; } virtual int mode(){return 1;} };
template<class T> T convert(T x) { return x; }
template<> int convert<int>(int x) { return x; }
int overload(int x) {return x;}
long overload(long x) {return x;}
#define DECLARE(name) int name(){return 0;}
DECLARE(expanded)
int use(Controller &c, int (*callback)()) { return c.step(convert(1))+overload(1)+callback()+c.mode(); }
}
