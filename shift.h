/* shift.h
 * --------------------------------
 * Declaration of fast shift operations for Numeric objects
 * Used in multiplication, division and some scientific functions
 */

#ifndef CALSTACK_SHIFT_H_
#define CALSTACK_SHIFT_H_

#include "numeric_cfg.h"

// Note: The directions (left or right) descripts the how DIGITS will shift rather than the POINT
Numeric *lshift(const Numeric *numeric);    // Left shift CALSTACK_FIXED_POINT units
Numeric *lshift_n(const Numeric *numeric, size_t n);   // Left shift n units
Numeric *rshift_twice(Numeric *numeric);    // Right shift 2 * CALSTACK_FIXED_POINT units
Numeric *rshift_n(Numeric *numeric, size_t n);  // Right shift n units
Numeric *rshift_frac(Numeric *numeric, size_t n); // Right shift, only for pure fractions
Numeric *shift_lscale(const Numeric *numeric, size_t n); // Multiply by a certain scale



#endif //CALSTACK_SHIFT_H_
