/** @req R-NAV
 * @baseline BL-DEMO-1 */
int navigate(int enabled) {
  int *p = 0;
  if (enabled) goto done;
  return *p; /* Intentional static-analysis defect. */
done:
  return enabled;
}
