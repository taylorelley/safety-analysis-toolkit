// CHECKS: misra-c2012-rule-15-4
extern int ExitNow(int);
extern void KeepGoing(int);
void f(int limit) {
  int x, y;
  for (x = 0; x < limit; ++x) {
    if (ExitNow(x)) {
      break;
    }
    for (y = 0; y < x; ++y) {
      if (ExitNow(limit - y)) {
        break;
      }
    }
  }
  for (x = 0; x < limit; ++x) { // NONCOMPLIANT
    if (ExitNow(x)) {
      break;
    } else if (ExitNow(-x)) {
      goto EXIT;
    } else {
      KeepGoing(x);
    }
  }
EXIT:;
}
