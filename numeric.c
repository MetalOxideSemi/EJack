/* numeric.h
 * --------------------------------
 * Implementation of basic operations including allocation, release, comparison and
 * fundamental mathematical operations of Numeric type
 */

#include "numeric.h"
#include <stdlib.h>

void Numeric_new(Numeric *numeric) {
    numeric->data = malloc(CALSTACK_INIT_LENGTH * sizeof(Udbyte));
    memset(numeric->data, 0, CALSTACK_INIT_LENGTH * sizeof(Udbyte));
    numeric->nbytes = CALSTACK_INIT_LENGTH;
    memset(numeric->fraction, 0, CALSTACK_FIXED_POINT * sizeof(Udbyte));
    numeric->sign = POS;
}

void Numeric_new_len(Numeric *numeric, size_t len) {
    numeric->data = malloc(len * sizeof(Udbyte));
    memset(numeric->data, 0, len * sizeof(Udbyte));
    numeric->nbytes = len;
    memset(numeric->fraction, 0, CALSTACK_FIXED_POINT * sizeof(Udbyte));
    numeric->sign = POS;
}

void Numeric_free(Numeric *numeric) {
    free(numeric->data);
    numeric->data = NULL;
    numeric->nbytes = 0;
}

Numeric *Numeric_assign(Numeric *tar, const Numeric *other) {
    free(tar->data);
    tar->sign = other->sign;
    tar->data = malloc(other->nbytes * sizeof(Udbyte));
    memcpy(tar->data, other->data, other->nbytes * sizeof(Udbyte));
    tar->nbytes = other->nbytes;
    memcpy(tar->fraction, other->fraction, CALSTACK_FIXED_POINT * sizeof(Udbyte));
    return tar;
}

Numeric *Numeric_move(Numeric *tar, Numeric *other) {
    free(tar->data);
    tar->sign = other->sign;
    tar->nbytes = other->nbytes;
    tar->data = other->data;
    other->data = NULL;
    memcpy(tar->fraction, other->fraction, CALSTACK_FIXED_POINT * sizeof(Udbyte));
    return tar;
}

bool Numeric_abs_less(const Numeric *left,
                      const Numeric *right) {  // Returns true if |left| < |right|, used to simplify subtraction
    if (left->nbytes < right->nbytes) {
        return true;
    }
    if (left->nbytes > right->nbytes) {
        return false;
    }

    Udbyte *l = left->data + left->nbytes - 1, *r = right->data + right->nbytes - 1;
    for (; l >= left->data; --l, --r) {
        if (*l < *r) {
            return true;
        }
        if (*r < *l) {
            return false;
        }

    }

    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (left->fraction[i] < right->fraction[i]) {
            return true;
        }
        if (left->fraction[i] > right->fraction[i]) {
            return false;
        }
    }

    return false;
}

static const Numeric *pnegone;
static const Numeric *precunit;
static const Numeric *calcu;

void Numeric_global_init() {
    pnegone = malloc(sizeof(Numeric));
    Numeric_new((Numeric *) pnegone);
    ((Numeric *) pnegone)->data[0] = 1;
    ((Numeric *) pnegone)->sign = NEG;

    precunit = malloc(sizeof(Numeric));
    Numeric_new((Numeric *) precunit);
    ((Numeric *) precunit)->fraction[CALSTACK_FIXED_POINT - 1] = 1;

    calcu = malloc(sizeof(Numeric));
    Numeric_new((Numeric *) calcu);
    ((Numeric *) calcu)->fraction[CALSTACK_FIXED_POINT / 3] = 1;

    prec_opt = true;
}

const Numeric *Numeric_negative_one() {
    return pnegone;
}

const Numeric *Numeric_precision() {
    return precunit;
}

const Numeric *Numeric_calculus() {
    return calcu;
}

void Numeric_free_ptr(Numeric *numeric) {
    Numeric_free(numeric);
    free(numeric);
}

Numeric *Numeric_neg(Numeric *tar) {
    tar->sign = !(tar->sign);
    return tar;
}

bool Numeric_is_zero(const Numeric *numeric) {
    if (numeric->nbytes > 1 || numeric->data[0] != 0) {
        return false;
    } else {
        for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
            if (numeric->fraction[i]) {
                return false;
            }
        }
    }
    return true;
}

bool Numeric_is_one(const Numeric *numeric) {
    if (numeric->nbytes > 1 || numeric->data[0] != 1 || numeric->sign != POS) {
        return false;
    } else {
        for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
            if (numeric->fraction[i]) {
                return false;
            }
        }
    }
    return true;
}

bool Numeric_is_neg_one(const Numeric *numeric) {
    if (numeric->nbytes > 1 || numeric->data[0] != 1 || numeric->sign != NEG) {
        return false;
    } else {
        for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
            if (numeric->fraction[i]) {
                return false;
            }
        }
    }
    return true;
}

Numeric *Numeric_add(const Numeric *left, const Numeric *right) {
#ifdef DEBUG
    printf("ADD %s + %s\n", Numerictostr(left), Numerictostr(right));
#endif

    // Addition of a positive number and a negative number will be delegated to subtraction function
    bool need_to_flip = false;
    if (left->sign == POS && right->sign == NEG) {
        Numeric temp;
        Numeric_new(&temp);
        Numeric_assign(&temp, right);
        Numeric_neg(&temp);
        Numeric *ret = Numeric_sub(left, &temp);
        Numeric_free(&temp);
        return ret;
    } else if (left->sign == NEG && right->sign == POS) {
        return Numeric_add(right, left);
    } else if (left->sign == NEG && right->sign == NEG) {
        need_to_flip = true;
    }
    Numeric *ret = malloc(sizeof(Numeric));
    const Numeric *longer, *shorter;
    left->nbytes > right->nbytes ? (longer = left, shorter = right)
                                 : (shorter = left, longer = right);
    Numeric_new_len(ret, longer->nbytes + 1);
    Udbyte carry = 0;
    for (int i = CALSTACK_FIXED_POINT - 1; i >= 0; --i) {
        ret->fraction[i] = longer->fraction[i] + shorter->fraction[i] + carry;
        carry = ret->fraction[i] / CALSTACK_UDBYTE_MAX;
        ret->fraction[i] %= CALSTACK_UDBYTE_MAX;
    }
    int i = 0;
    for (; i < shorter->nbytes; ++i) {
        ret->data[i] = longer->data[i] + shorter->data[i] + carry;
        carry = ret->data[i] / CALSTACK_UDBYTE_MAX;
        ret->data[i] %= CALSTACK_UDBYTE_MAX;
    }
    for (; i < longer->nbytes; ++i) {
        ret->data[i] = longer->data[i] + carry;
        carry = ret->data[i] / CALSTACK_UDBYTE_MAX;
        ret->data[i] %= CALSTACK_UDBYTE_MAX;
    }
    ret->sign = need_to_flip;
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    return ret;
}

Numeric *Numeric_sub(const Numeric *left, const Numeric *right) {
#ifdef DEBUG
    printf("SUB %s(nbytes=%llu) - %s(nbytes=%llu)\n", Numerictostr(left), left->nbytes, Numerictostr(right),
           right->nbytes);
#endif
    // If left < right, delegate the operation to Numeric_sub(right,left) and opposite the sign
    if (Numeric_abs_less(left, right)) {
        Numeric *ret = Numeric_sub(right, left);
        Numeric_neg(ret);
        return ret;
    }
    bool need_to_flip = false;
    // Subtraction a negative number from a positive one will be delegated to addition function
    if (left->sign == POS && right->sign == NEG) {
        Numeric temp;
        Numeric_new(&temp);
        Numeric_assign(&temp, right);
        Numeric_neg(&temp);
        Numeric *ret = Numeric_add(left, &temp);
        Numeric_free(&temp);
        return ret;
    } else if (left->sign == NEG && right->sign == POS) {
        Numeric temp;
        Numeric_new(&temp);
        Numeric_assign(&temp, left);
        Numeric_neg(&temp);
        Numeric *ret = Numeric_add(right, &temp);
        Numeric_free(&temp);
        Numeric_neg(ret);
        return ret;
    } else if (left->sign == NEG && right->sign == NEG) {
        need_to_flip = true;
    }
    Numeric *ret = malloc(sizeof(Numeric));
    Numeric_new_len(ret, left->nbytes);
    Udbyte carry = 0;
    for (int i = CALSTACK_FIXED_POINT - 1; i > 0; --i) {
        ret->fraction[i] += left->fraction[i] + CALSTACK_UDBYTE_MAX - right->fraction[i] + carry;
        carry = ret->fraction[i] / CALSTACK_UDBYTE_MAX;
        ret->fraction[i] %= CALSTACK_UDBYTE_MAX;
        --ret->fraction[i - 1];
    }
    ret->fraction[0] += left->fraction[0] + CALSTACK_UDBYTE_MAX - right->fraction[0] + carry;
    carry = ret->fraction[0] / CALSTACK_UDBYTE_MAX;
    ret->fraction[0] %= CALSTACK_UDBYTE_MAX;
    int i = 0;
    for (; i < right->nbytes; ++i) {
        ret->data[i] = left->data[i] + CALSTACK_UDBYTE_MAX - right->data[i] + carry - 1;
        carry = ret->data[i] / CALSTACK_UDBYTE_MAX;
        ret->data[i] %= CALSTACK_UDBYTE_MAX;
    }
    for (; i < left->nbytes; ++i) {
        ret->data[i] = left->data[i] + CALSTACK_UDBYTE_MAX + carry - 1;
        carry = ret->data[i] / CALSTACK_UDBYTE_MAX;
        ret->data[i] %= CALSTACK_UDBYTE_MAX;
    }

    ret->sign = need_to_flip;
    while ((!ret->data[ret->nbytes - 1]) && ret->nbytes > 1) {
        --ret->nbytes;
    }
    return ret;
}






