#include <assert.h>
/** @req R-NAV
 * @baseline BL-DEMO-1 */
int navigate(int enabled) {
  int *p = 0;
  if (enabled) goto done;
  return *p; /* Intentional static-analysis defect. */
done:
  return enabled;
}
/** @req R-NAV */
int main(void) { assert(navigate(1) == 1); return 0; }
