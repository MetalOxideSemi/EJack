/* numeric_cfg.h
 * Created by Wang Haoren
 * --------------------------------
 * Declaration of the high-precision Numeric data type
 * Configurations of the memory layout
 * and the fundamental settings of precision
 */

#ifndef CALSTACK_NUMERIC_CFG_H_
#define CALSTACK_NUMERIC_CFG_H_


#define CALSTACK_INIT_LENGTH 1        // The initial length when Numeric_new is called
#define CALSTACK_UDBYTE_LENGTH 5      // Length of each byte
#define CALSTACK_FIXED_POINT 12       // Precision of fixed-point fraction
#define CALSTACK_UDBYTE_MAX 100000ull  // Max value of each byte
#define CALSTACK_NBYTES_MAX 16777216     // Max length
#define CALSTACK_OUTPUT_FIX "%05llu"  // Output format

#include <stddef.h>                  // For size_t

#define int64 long long

typedef unsigned int64 Udbyte;   // In x64 architecture, arithmetic of 64-bit integer is faster than 32-bit and keeps higher precision

typedef struct {
    int sign;                      // Sign field of a numeric, with macro POS and NEG as its only legal values.
    // Other values suggest other type of data (closures)
    Udbyte *data;                  // Pointer to the integral part; Allows dynamic memory allocation
    size_t nbytes;                 // Counts of units that data pints to, i.e. the length of a numeric
    Udbyte fraction[CALSTACK_FIXED_POINT];  // Fractional part
} Numeric;


#define POS 0
#define NEG 1

#endif