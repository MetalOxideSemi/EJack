/* div.c
 * Created by Wang Haoren
 * --------------------------------
 * Implementation of high-precision division
 */

#include "numeric.h"
#include "shift.h"

#include <stdlib.h>


Numeric *partial_mult(Udbyte left, const Numeric *right) {
#ifdef DEBUG_2
    printf("PARTIAL MULT %llu * %s\n", left, Numerictostr(right));
#endif
    Numeric *ret = malloc(sizeof(Numeric));
    Numeric_new_len(ret, right->nbytes + 1);
    Udbyte carry = 0;
    for (size_t i = 0; i < right->nbytes; ++i) {
        ret->data[i] = right->data[i] * left + carry;
        carry = ret->data[i] / CALSTACK_UDBYTE_MAX;
        ret->data[i] %= CALSTACK_UDBYTE_MAX;
    }
    if (carry) {
        ret->data[right->nbytes] = carry;
    } else {
        ret->nbytes = right->nbytes;
    }
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    return ret;
}

void partial_sub(const Numeric *left, const Numeric *right, size_t pos, Udbyte *quotient, Numeric *rest) {
#ifdef DEBUG_2
    printf("PARTIAL SUB %s - %s * %llu^%llu\n", Numerictostr(left), Numerictostr(right), CALSTACK_UDBYTE_MAX, pos);
#endif
    Numeric *shift = shift_lscale(right, pos), *tmp2;
    if (Numeric_less(left, shift)) {
        *quotient = 0;
        Numeric_assign(rest, left);
        Numeric_free_ptr(shift);
        return;
    }
    Udbyte mid, upper = CALSTACK_UDBYTE_MAX - 1, lower = 0;
    while (lower <= upper) {
        mid = (lower + upper) / 2;
        switch (Numeric_compare(tmp2 = partial_mult(mid, shift), left)) {
            case -1:
                lower = mid + 1;
                break;
            case 0 :
                lower = mid + 1;
                goto out_of_while;
            default:
                upper = mid - 1;
        }
        Numeric_free(tmp2);
    }
    Numeric *tmp;
    out_of_while:
    tmp = Numeric_sub(left, tmp2 = partial_mult(--lower, shift));
    Numeric_move(rest, tmp);
    *quotient = lower;
#ifdef DEBUG_2
    printf("Quotient = %llu, Rest = %s\n", *quotient, Numerictostr(rest));
#endif
    free(tmp);
    Numeric_free_ptr(tmp2);
}

Numeric *Numeric_div(const Numeric *left, const Numeric *right) {  // That's AWFUL !!
    Numeric *ret = malloc(sizeof(Numeric));
#ifdef DEBUG
    printf("DIV %s / %s\n", Numerictostr(left), Numerictostr(right));
#endif
    if (Numeric_is_zero(left)) {
        Numeric_new(ret);
        return ret;
    }
    if (Numeric_is_zero(right)) {
        puts("CRASHED: CANNOT DIVIDED BY ZERO!");
        exit(1);
    }
    if (Numeric_is_neg_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        Numeric_neg(ret);
        return ret;
    }
    if (Numeric_is_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        return ret;
    }

    Numeric rest, restn;
    Numeric_new(&rest), Numeric_new(&restn);
    int64 extra_shift_candi = (int64) right->nbytes - (int64) left->nbytes;
    int64 extra_shift = extra_shift_candi > CALSTACK_FIXED_POINT ? extra_shift_candi - CALSTACK_FIXED_POINT : 0;
    Numeric *left_cpy = lshift_n(left, 2 * CALSTACK_FIXED_POINT + extra_shift),
            *right_cpy = lshift_n(right, CALSTACK_FIXED_POINT);
    //const Numeric *left_cpy = left, *right_cpy = right;
    Numeric_assign(&rest, left_cpy);
    int64 diff = left_cpy->nbytes - right_cpy->nbytes;
    Numeric_new_len(ret, diff > 0 ? diff + 1 : 1);
    for (int64 i = diff; i >= 0; --i) {
        partial_sub(&rest, right_cpy, i, ret->data + i, &restn);
        Numeric_move(&rest, &restn);
    }
    Numeric *tmp = ret;
    ret = rshift_n(ret, CALSTACK_FIXED_POINT + extra_shift);
    Numeric_free_ptr(tmp);
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    ret->sign = left->sign ^ right->sign;
    return ret;
}

Numeric *Numeric_div_int(const Numeric *left, const Numeric *right) {  // That's AWFUL !!
    Numeric *ret = malloc(sizeof(Numeric));
#ifdef DEBUG
    printf("DIV %s / %s\n", Numerictostr(left), Numerictostr(right));
#endif
    if (Numeric_is_zero(left)) {
        Numeric_new(ret);
        return ret;
    }
    if (Numeric_is_zero(right)) {
        puts("CRASHED: CANNOT DIVIDED BY ZERO!");
        exit(1);
    }
    if (Numeric_is_neg_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        Numeric_neg(ret);
        return ret;
    }
    if (Numeric_is_one(right)) {
        Numeric_new(ret);
        Numeric_assign(ret, left);
        return ret;
    }

    Numeric rest, restn;
    Numeric_new(&rest), Numeric_new(&restn);
    const Numeric *left_cpy = left, *right_cpy = right;
    Numeric_assign(&rest, left_cpy);
    size_t diff = left_cpy->nbytes - right_cpy->nbytes;
    Numeric_new_len(ret, diff > 0 ? diff + 1 : 1);
    for (int64 i = diff; i >= 0; --i) {
        partial_sub(&rest, right_cpy, i, ret->data + i, &restn);
        Numeric_move(&rest, &restn);
    }
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    ret->sign = left->sign ^ right->sign;
    return ret;
}