#include <assert.h>
#include "../src/navigation.c"
/** @test T-NAV
 * @req R-NAV */
int main(void) { assert(navigate(1) == 1); return 0; }
