/* mult.c
 * --------------------------------
 * Implementation of high-precision multiplication
 */

#include "numeric.h"
#include "shift.h"
#include "mult_fft_helper.h"
#include <stdlib.h>

bool prec_opt;

static const Udbyte P10[] = {  // Power of 10
        1ull,
        10ull,
        100ull,
        1000ull,
        10000ull,
        100000ull,
        1000000ull,
        10000000ull,
        100000000ull,
        1000000000ull
};


bool has_frac(const Numeric *numeric) {
    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (numeric->fraction[i]) {
            return true;
        }
    }
    return false;
}

Numeric *Numeric_mult(const Numeric *left, const Numeric *right) {
    Numeric *ret = malloc(sizeof(Numeric));
#ifdef DEBUG
    //printf("MULT %s * %s\n", Numerictostr(left), Numerictostr(right));
#endif
    // For the special cases of x*0, x*1 and x*-1, no actual calculation needed.
    if (Numeric_is_zero(left) || Numeric_is_zero(right)) {
        Numeric_new(ret);
        return ret;
    }
    if (Numeric_is_neg_one(left)) {
        Numeric_new(ret);
        Numeric_assign(ret, right);
        Numeric_neg(ret);
        return ret;
    }
    if (Numeric_is_neg_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        Numeric_neg(ret);
        return ret;
    }
    if (Numeric_is_one(left)) {
        Numeric_new(ret);
        Numeric_assign(ret, right);
        return ret;
    }
    if (Numeric_is_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        return ret;
    }

    // When multiplying integers, we don't need to shift to calculate the fractions.
    if (has_frac(left) || has_frac(right)) {
        Numeric tret;
        Numeric_new_len(&tret, left->nbytes + right->nbytes + 2 * CALSTACK_FIXED_POINT);
        Numeric *left_cpy = lshift(left), *right_cpy = lshift(right);
        size_t brtable_len = align_p2(left_cpy->data, right_cpy->data, left_cpy->nbytes, right_cpy->nbytes);
        unsigned int64 *brtable = malloc(brtable_len * sizeof(unsigned int64));
        bitrevtb(brtable, brtable_len);
        fourier_transform(brtable_len, brtable);  // Use Fast Fourier Transformation to optimize multiplication
        Uldbyte *temp = malloc(tret.nbytes * sizeof(Uldbyte));
        result_copy(temp, tret.nbytes);
        for (size_t i = 0; i < tret.nbytes; ++i) {
            tret.data[i] = (Udbyte) temp[i];
        }
        while ((!tret.data[tret.nbytes - 1]) && tret.nbytes > 1) {  // Fix the leading zeros
            --tret.nbytes;
        }
        free(brtable), free(temp);
        ret = rshift_twice(&tret);
    } else {
        // Shift the fraction parts to integral parts to calculate
        Numeric_new_len(ret, left->nbytes + right->nbytes);
        size_t brtable_len = align_p2(left->data, right->data, left->nbytes, right->nbytes);
        unsigned int64 *brtable = malloc(brtable_len * sizeof(unsigned int64));
        bitrevtb(brtable, brtable_len);
        fourier_transform(brtable_len, brtable);
        Uldbyte *temp = malloc(ret->nbytes * sizeof(Uldbyte));
        result_copy(temp, ret->nbytes);
        for (size_t i = 0; i < ret->nbytes; ++i) {
            ret->data[i] = (Udbyte) temp[i];
        }
        while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
            --ret->nbytes;
        }
        free(brtable), free(temp);
    }
    if (ret->fraction[CALSTACK_FIXED_POINT - 1] > CALSTACK_UDBYTE_MAX / 2 && prec_opt) {
        const Numeric *precunit = malloc(sizeof(Numeric));
        Numeric_new((Numeric *) precunit);
        ((Numeric *) precunit)->fraction[CALSTACK_FIXED_POINT - 1] =
                CALSTACK_UDBYTE_MAX - ret->fraction[CALSTACK_FIXED_POINT - 1];
        Numeric *tmp = ret;
        ret = Numeric_add(ret, precunit);
        Numeric_free_ptr(tmp);
    }
    ret->sign = left->sign ^ right->sign;
    return ret;
}