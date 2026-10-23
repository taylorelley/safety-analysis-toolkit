#include <assert.h>
namespace controller {
template<class T> T apply(T value) { return value; }
int apply(int a, int b) { return a + b; }
}
/** @req R-CTRL */
int main() { int x = controller::apply(0); assert(x == 1); return 0; }
