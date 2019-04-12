/* ioconv.c
 * Created by Wang Haoren
 * --------------------------------
 * Implementation of conversion between string and Numeric objects
 */

#include "numeric.h"
#include <stdlib.h>
#include <string.h>

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

Udbyte fastconvert(const char *start, const char *end) {
    Udbyte ret = 0;
    while (start < end) {
        ret *= 10;
        ret += *start++ - '0';
    }
    return ret;
}

bool strtoNumeric(const char *str, Numeric *numeric) {
    Numeric_free(numeric);
    Numeric_new(numeric);
    numeric->sign = POS;
    while (*str == '+' || *str == '-') {
        if (*str == '-') {
            numeric->sign = !numeric->sign;
        }
        ++str;
    }
    size_t len = strlen(str);
    const char *const p2p = strchr(str, '.');
    if (p2p) {
        size_t ilen = p2p - str;
        size_t dlen = len - ilen - 1;
        if (dlen > CALSTACK_FIXED_POINT * CALSTACK_UDBYTE_LENGTH) {
            dlen = CALSTACK_FIXED_POINT * CALSTACK_UDBYTE_LENGTH;
        }
        numeric->nbytes = ilen / CALSTACK_UDBYTE_LENGTH + (!!(ilen % CALSTACK_UDBYTE_LENGTH));
        numeric->data = malloc(numeric->nbytes * sizeof(Udbyte));
        const char *current = p2p;
        for (size_t i = 0; i < numeric->nbytes - 1; ++i) {
            current -= CALSTACK_UDBYTE_LENGTH;
            numeric->data[i] = fastconvert(current, current + CALSTACK_UDBYTE_LENGTH);
        }
        numeric->data[numeric->nbytes - 1] = fastconvert(str, current);


        size_t nd = dlen / CALSTACK_UDBYTE_LENGTH + (!!(dlen % CALSTACK_UDBYTE_LENGTH));
        if (nd > CALSTACK_FIXED_POINT) {
            return false;
        }
        for (size_t i = 0; i < nd - 1; ++i) {
            numeric->fraction[i] = fastconvert(p2p + CALSTACK_UDBYTE_LENGTH * i + 1,
                                               p2p + CALSTACK_UDBYTE_LENGTH * i + 1 + CALSTACK_UDBYTE_LENGTH);
        }
        numeric->fraction[nd - 1] = fastconvert(p2p + CALSTACK_UDBYTE_LENGTH * (nd - 1) + 1,
                                                str + len);
        size_t fix = dlen % CALSTACK_UDBYTE_LENGTH;
        if (fix) {
            numeric->fraction[nd - 1] *= P10[CALSTACK_UDBYTE_LENGTH - fix];
        }
    } else {
        size_t ilen = len;
        numeric->nbytes = ilen / CALSTACK_UDBYTE_LENGTH + (!!(ilen % CALSTACK_UDBYTE_LENGTH));
        numeric->data = malloc(numeric->nbytes * sizeof(Udbyte));
        const char *current = str + ilen;
        for (size_t i = 0; i < numeric->nbytes - 1; ++i) {
            current -= CALSTACK_UDBYTE_LENGTH;
            numeric->data[i] = fastconvert(current, current + CALSTACK_UDBYTE_LENGTH);
        }
        numeric->data[numeric->nbytes - 1] = fastconvert(str, current);
    }
    return true;
}

char *Numerictostr(const Numeric *numeric) {
    char *res = malloc(CALSTACK_UDBYTE_LENGTH * (CALSTACK_FIXED_POINT + numeric->nbytes)), *dst = res;
    if (numeric->sign == NEG) {
        *dst++ = '-';
    }
    dst += sprintf(dst, "%llu", numeric->data[numeric->nbytes - 1]);
    for (size_t i = numeric->nbytes - 1; i > 0; --i) {
        dst += sprintf(dst, CALSTACK_OUTPUT_FIX, numeric->data[i - 1]);
    }
    int last = CALSTACK_FIXED_POINT - 1;
    for (; last >= 0; --last) {
        if (numeric->fraction[last]) {
            break;
        }
    }
    if (last >= 0) {
        *dst++ = '.';
        for (int i = 0; i <= last; ++i) {
            dst += sprintf(dst, CALSTACK_OUTPUT_FIX, numeric->fraction[i]);
        }
        while (*--dst == '0');
        *++dst = '\0';
    }
    return res;
}