// CHECKS: misra-c2012-dir-4-5
int id1_a_b_c;
int id1_abc; // NONCOMPLIANT
int id2_abc;
int id2_ABC; // NONCOMPLIANT
int id4_I;
int id4_1; // NONCOMPLIANT
int id8_rn;
int id8_m; // NONCOMPLIANT
int distinct_name;
struct holder { int id9_m; };
int id9_rn;
void f(int lO0) {
  int l00 = lO0; // NONCOMPLIANT
  (void)l00;
}
