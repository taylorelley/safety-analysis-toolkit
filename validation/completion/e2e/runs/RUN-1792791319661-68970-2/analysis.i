# 1 "/workspace/safety/build/demo/harness/navigation_harness.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 399 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/workspace/safety/build/demo/harness/navigation_harness.c" 2
# 1 "/workspace/scratch/frama/usr/share/frama-c/libc/assert.h" 1
/**************************************************************************/
/*                                                                        */
/*  This file is part of Frama-C.                                         */
/*                                                                        */
/*  Copyright (C) 2007-2022                                               */
/*    CEA (Commissariat à l'énergie atomique et aux énergies              */
/*         alternatives)                                                  */
/*                                                                        */
/*  you can redistribute it and/or modify it under the terms of the GNU   */
/*  Lesser General Public License as published by the Free Software       */
/*  Foundation, version 2.1.                                              */
/*                                                                        */
/*  It is distributed in the hope that it will be useful,                 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*  GNU Lesser General Public License for more details.                   */
/*                                                                        */
/*  See the GNU Lesser General Public License version 2.1                 */
/*  for more details (enclosed in the file licenses/LGPLv2.1).            */
/*                                                                        */
/**************************************************************************/



# 1 "/workspace/scratch/frama/usr/share/frama-c/libc/features.h" 1
/**************************************************************************/
/*                                                                        */
/*  This file is part of Frama-C.                                         */
/*                                                                        */
/*  Copyright (C) 2007-2022                                               */
/*    CEA (Commissariat à l'énergie atomique et aux énergies              */
/*         alternatives)                                                  */
/*                                                                        */
/*  you can redistribute it and/or modify it under the terms of the GNU   */
/*  Lesser General Public License as published by the Free Software       */
/*  Foundation, version 2.1.                                              */
/*                                                                        */
/*  It is distributed in the hope that it will be useful,                 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*  GNU Lesser General Public License for more details.                   */
/*                                                                        */
/*  See the GNU Lesser General Public License version 2.1                 */
/*  for more details (enclosed in the file licenses/LGPLv2.1).            */
/*                                                                        */
/**************************************************************************/




// *** Definitions to improve compatibility with GCC-specific built-ins
// and GNU-based code ***
# 63 "/workspace/scratch/frama/usr/share/frama-c/libc/features.h"
// Frama-C does not support GCC's __builtin_object_size.
// To improve compatibility with some codebases,
// we define it anyway, but it always returns -1, as if
// the compiler were unable to statically determine
// the object size (we only consider the cases where type
// is either 0 or 1).
// Note that for some built-ins, we force them to our definition,
// while others we leave unmodified if they exist
# 118 "/workspace/scratch/frama/usr/share/frama-c/libc/features.h"
// When linking code including Frama-C's libc, we avoid adding 'extern'
// to some variable declarations. In this case, __FC_EXTERN is defined to
// the empty string. Otherwise, define it to 'extern'.




// C11 §6.10.8.3 Conditional feature macros: Frama-C does not support complex.h




/* end __FC_FEATURES_H */
# 26 "/workspace/scratch/frama/usr/share/frama-c/libc/assert.h" 2
#pragma fc_stdlib(push,"/workspace/scratch/frama/usr/share/frama-c/libc/assert.h")



/*@
  requires nonnull_c: c != 0;
  terminates c != 0;
  assigns \nothing;
*/
extern void __FC_assert(int c, const char* file, int line, const char*expr);





 #pragma fc_stdlib(pop)
# 2 "/workspace/safety/build/demo/harness/navigation_harness.c" 2
# 1 "/workspace/safety/build/demo/harness/../src/navigation.c" 1
/** @req R-NAV
 * @baseline BL-DEMO-1 */
int navigate(int enabled) {
  int *p = 0;
  if (enabled) goto done;
  return *p; /* Intentional static-analysis defect. */
done:
  return enabled;
}
# 3 "/workspace/safety/build/demo/harness/navigation_harness.c" 2
/** @test T-NAV
 * @req R-NAV */
int main(void) { (__FC_assert((navigate(1) == 1) != 0,__FC_FILENAME__,5,"navigate(1) == 1")); return 0; }
