/* scientific.c
 * --------------------------------
 * The implementation of scientific functions
 */

#ifndef CALSTACK_SCIENTIFIC_H
#define CALSTACK_SCIENTIFIC_H

#include "numeric.h"
#include "operation.h"

void global_table_init();

Numeric *Numeric_pow(const Numeric *base, const Numeric *power);

Numeric *Numeric_sin(const Numeric *x);

Numeric *Numeric_cos(const Numeric *x);

Numeric *Numeric_tan(const Numeric *x);

Numeric *Numeric_sec(const Numeric *x);

Numeric *Numeric_csc(const Numeric *x);

Numeric *Numeric_cot(const Numeric *x);

Numeric *Numeric_arcsin(const Numeric *x);

Numeric *Numeric_arccos(const Numeric *x);

Numeric *Numeric_arctan(const Numeric *x);

Numeric *Numeric_arcsec(const Numeric *x);

Numeric *Numeric_arccsc(const Numeric *x);

Numeric *Numeric_arccot(const Numeric *x);

Numeric *Numeric_sinh(const Numeric *x);

Numeric *Numeric_cosh(const Numeric *x);

Numeric *Numeric_tanh(const Numeric *x);

Numeric *Numeric_sech(const Numeric *x);

Numeric *Numeric_csch(const Numeric *x);

Numeric *Numeric_coth(const Numeric *x);

Numeric *Numeric_arcsinh(const Numeric *x);

Numeric *Numeric_arccosh(const Numeric *x);

Numeric *Numeric_arctanh(const Numeric *x);

Numeric *Numeric_arcsech(const Numeric *x);

Numeric *Numeric_arccsch(const Numeric *x);

Numeric *Numeric_arccoht(const Numeric *x);

Numeric *Numeric_log(const Numeric *base, const Numeric *oper);

Numeric *Numeric_log2(const Numeric *x);

Numeric *Numeric_ln(const Numeric *x);

Numeric *Numeric_lg(const Numeric *x);

Numeric *Numeric_sqrt(const Numeric *x);

Numeric *Numeric_exp(const Numeric *x);

Numeric *Numeric_abs(const Numeric *x);

Numeric *Numeric_fact(const Numeric *x);

#endif //CALSTACK_SCIENTIFIC_H
