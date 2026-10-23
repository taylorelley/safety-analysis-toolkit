# 1 "/workspace/safety/build/demo/src/contract.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 399 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/workspace/safety/build/demo/src/contract.c" 2
/* SPDX-License-Identifier: Apache-2.0 */
/** @req R-NAV */
/*@ assigns \nothing; ensures \result == x; */
int identity(int x) { return x; }
