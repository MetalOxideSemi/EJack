/* numeric.h
 * --------------------------------
 * Declaration of basic operations including allocation, release, comparison and
 * fundamental mathematical operations of Numeric type
 */


//#define DEBUG  // A switch that controls whether the operators display debug messages
#ifndef CALSTACK_NUMERIC_H_
#define CALSTACK_NUMERIC_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "numeric_cfg.h"

extern bool prec_opt;

void Numeric_global_init(); // Initialize global environment

void Numeric_new(Numeric *numeric);  // Initialize a Numeric object. Usage: Numeric num; Numeric_new(&num);
void Numeric_new_len(Numeric *numeric,
                     size_t len);  // Initialize a Numeric object with given length. Usage: Numeric num; Numeric_new_len(&num, 10);
void Numeric_free(Numeric *numeric); // Release a Numeric object
void Numeric_free_ptr(Numeric *numeric);  // Release a numeric object on heap

Numeric *Numeric_assign(Numeric *tar, const Numeric *other);           // =
Numeric *Numeric_move(Numeric *tar, Numeric *other);             // = (rvalue)

Numeric *Numeric_neg(Numeric *tar);                              // Convert a numeric object to its opposite value.

Numeric *Numeric_add(const Numeric *left, const Numeric *right);        // +
Numeric *Numeric_sub(const Numeric *left, const Numeric *right);        // -
Numeric *Numeric_mult(const Numeric *left, const Numeric *right);       // *
Numeric *Numeric_div(const Numeric *left, const Numeric *right);        // /
Numeric *Numeric_div_int(const Numeric *left, const Numeric *right);

bool Numeric_less(const Numeric *left, const Numeric *right);          // >
bool Numeric_greater(const Numeric *left, const Numeric *right);       // <
bool Numeric_eq(const Numeric *left, const Numeric *right);            // ==
int Numeric_compare(const Numeric *left, const Numeric *right);

#define Numeric_not_eq(left, right) (!(Numeric_eq((left),(right))))        // !=
#define Numeric_greater_eq(left, right) (!(Numeric_less((left),(right))))  // >=
#define Numeric_less_eq(left, right) (!(Numeric_greater((left),(right))))  // <=
#define Numeric_mod2(numeric) ((numeric).data[0] & 1)

const Numeric *Numeric_negative_one();  // Returns -1 in Numeric type
const Numeric *Numeric_precision();     // Returns the minimum positive value that a Numeric can keep
const Numeric *Numeric_calculus();

bool has_frac(const Numeric *numeric);         // Returns true if numeric is not integer
bool Numeric_is_zero(const Numeric *numeric);  // Returns true if numeric == 0
bool Numeric_is_one(const Numeric *numeric);   // Returns true if numeric == 1
bool Numeric_is_neg_one(const Numeric *numeric);   // Returns true is numeric == -1
bool Numeric_sign(const Numeric *numeric);   // Returns numeric->sign
bool strtoNumeric(const char *str, Numeric *numeric);         // Convert a string to Numeric
char *Numerictostr(const Numeric *numeric);                  // Convert a Numeric to string

#endif //CALSTACK_NUMERIC_H
