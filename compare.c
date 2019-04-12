#include "numeric.h"

bool Numeric_less(const Numeric *left, const Numeric *right) {
    if (left->sign == NEG && right->sign == POS) {
        return true;
    }
    if (left->sign == POS && right->sign == NEG) {
        return false;
    }
    if (left->nbytes < right->nbytes) {
        return left->sign == POS ? true : false;
    }
    if (left->nbytes > right->nbytes) {
        return left->sign == POS ? false : true;
    }

    Udbyte *l = left->data + left->nbytes - 1, *r = right->data + right->nbytes - 1;
    for (; l >= left->data; --l, --r) {
        if (*l < *r) {
            return left->sign == POS ? true : false;
        }
        if (*r < *l) {
            return left->sign == POS ? false : true;
        }

    }

    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (left->fraction[i] < right->fraction[i]) {
            return left->sign == POS ? true : false;
        }
        if (left->fraction[i] > right->fraction[i]) {
            return left->sign == POS ? false : true;
        }
    }

    return left->sign == POS ? false : true;

}

bool Numeric_greater(const Numeric *left, const Numeric *right) {
    if (left->sign == NEG && right->sign == POS) {
        return false;
    }
    if (left->sign == POS && right->sign == NEG) {
        return true;
    }
    if (left->nbytes < right->nbytes) {
        return left->sign == POS ? false : true;
    }
    if (left->nbytes > right->nbytes) {
        return left->sign == POS ? true : false;
    }

    Udbyte *l = left->data + left->nbytes - 1, *r = right->data + right->nbytes - 1;
    for (; l >= left->data; --l, --r) {
        if (*l > *r) {
            return left->sign == POS ? true : false;
        }
        if (*r > *l) {
            return left->sign == POS ? false : true;
        }
    }

    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (left->fraction[i] > right->fraction[i]) {
            return left->sign == POS ? true : false;
        }
        if (left->fraction[i] < right->fraction[i]) {
            return left->sign == POS ? false : true;
        }
    }


    return left->sign == POS ? false : true;
}

bool Numeric_eq(const Numeric *left, const Numeric *right) {
    if (left->sign != right->sign || left->nbytes != right->nbytes) {
        return false;
    }
    Udbyte *l = left->data, *r = right->data;
    for (size_t i = 0; i < left->nbytes; ++i, ++l, ++r) {
        if (*l != *r) {
            return false;
        }
    }
    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (left->fraction[i] != right->fraction[i]) {
            return false;
        }
    }
    return true;
}

int Numeric_compare(const Numeric *left, const Numeric *right) {
    if (left->sign == NEG && right->sign == POS) {
        return -1;
    }
    if (left->sign == POS && right->sign == NEG) {
        return 1;
    }
    if (left->nbytes > right->nbytes) {
        return left->sign == POS ? 1 : -1;
    }
    if (left->nbytes < right->nbytes) {
        return left->sign == POS ? -1 : 1;
    }

    Udbyte *l = left->data + left->nbytes - 1, *r = right->data + right->nbytes - 1;
    for (; l >= left->data; --l, --r) {
        if (*l > *r) {
            return left->sign == POS ? 1 : -1;
        }
        if (*r > *l) {
            return left->sign == POS ? -1 : 1;
        }
    }

    for (size_t i = 0; i < CALSTACK_FIXED_POINT; ++i) {
        if (left->fraction[i] > right->fraction[i]) {
            return left->sign == POS ? 1 : -1;
        }
        if (left->fraction[i] < right->fraction[i]) {
            return left->sign == POS ? -1 : 1;
        }
    }

    return 0;
}