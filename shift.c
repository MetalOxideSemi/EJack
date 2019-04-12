/* shift.c
 * --------------------------------
 * Implementation of fast shift operations for Numeric objects
 */

#include "numeric.h"
#include "shift.h"

#include <stdlib.h>

Numeric *lshift(const Numeric *numeric) {
    Numeric *ret = malloc(sizeof(Numeric));
    Numeric_new_len(ret, numeric->nbytes + CALSTACK_FIXED_POINT);
    memcpy(ret->data + CALSTACK_FIXED_POINT, numeric->data, numeric->nbytes * sizeof(Udbyte));
    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        ret->data[i] = numeric->fraction[CALSTACK_FIXED_POINT - i - 1];
    }
    return ret;
}

Numeric *lshift_n(const Numeric *numeric, size_t n) {
    Numeric *ret = malloc(sizeof(Numeric));
    Numeric_new_len(ret, numeric->nbytes + n);
    memcpy(ret->data + n, numeric->data, numeric->nbytes * sizeof(Udbyte));
    if (n >= CALSTACK_FIXED_POINT) {
        for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
            ret->data[i + n - CALSTACK_FIXED_POINT] = numeric->fraction[CALSTACK_FIXED_POINT - i - 1];
        }
    } else {
        for (size_t i = 0; i < CALSTACK_FIXED_POINT - n; ++i) {
            ret->fraction[i] = numeric->fraction[i + n];
        }
        for (size_t i = 0; i < n; ++i) {
            ret->data[i] = numeric->fraction[n - i - 1];
        }
    }
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    return ret;
}

Numeric *rshift_twice(Numeric *numeric) {
    Numeric *ret = malloc(sizeof(Numeric));
    int64 candilen = numeric->nbytes - 2 * CALSTACK_FIXED_POINT;
    Numeric_new_len(ret,  (candilen < 1 ? 1 : candilen));
    if (numeric->nbytes > 2 * CALSTACK_FIXED_POINT) {
        memcpy(ret->data, numeric->data + 2 * CALSTACK_FIXED_POINT,
               (numeric->nbytes - 2 * CALSTACK_FIXED_POINT) * sizeof(Udbyte));
        for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
            ret->fraction[i] = numeric->data[2 * CALSTACK_FIXED_POINT - i - 1];
        }
    } else {
        for (size_t i = 2 * CALSTACK_FIXED_POINT - numeric->nbytes; i < CALSTACK_FIXED_POINT; ++i) {
            ret->fraction[i] = numeric->data[2 * CALSTACK_FIXED_POINT - i - 1];
        }
    }

    return ret;
}

Numeric *rshift_n(Numeric *numeric, size_t n) {
    Numeric *ret = malloc(sizeof(Numeric));
    if (numeric->nbytes > CALSTACK_FIXED_POINT) {
        Numeric_new_len(ret, numeric->nbytes - CALSTACK_FIXED_POINT);
        memcpy(ret->data, numeric->data + n,
               (numeric->nbytes - n) * sizeof(Udbyte));
    } else {
        Numeric_new_len(ret, 1);
    }
    for (size_t i = 0; i < n && i < numeric->nbytes; ++i) {
        ret->fraction[n - i - 1] = numeric->data[i];
    }
//    for (size_t i = n; i < CALSTACK_FIXED_POINT; ++i) {
//        ret->fraction[i] = numeric->fraction[i - n];
//    }
    return ret;
}

Numeric *rshift_frac(Numeric *numeric, size_t n) {
    Numeric *ret = malloc(sizeof(Numeric));
    if (numeric->nbytes > CALSTACK_FIXED_POINT) {
        Numeric_new_len(ret, numeric->nbytes - CALSTACK_FIXED_POINT);
        memcpy(ret->data, numeric->data + n,
               (numeric->nbytes - n) * sizeof(Udbyte));
    } else {
        Numeric_new_len(ret, 1);
    }
    size_t i = 0;
    for (; i < n && i < numeric->nbytes; ++i) {
        ret->fraction[n - i - 1] = numeric->data[i];
    }
    for (; i < CALSTACK_FIXED_POINT; ++i) {
        ret->fraction[i] = numeric->fraction[i - n];
    }
    return ret;
}

Numeric *shift_lscale(const Numeric *numeric, size_t n) {
    Numeric *ret = malloc(sizeof(Numeric));
    Numeric_new_len(ret, numeric->nbytes + n);
    for (size_t i = 0; i < numeric->nbytes; ++i) {
        ret->data[i + n] = numeric->data[i];
    }
    return ret;
}

