/* mult_fft_helper.h
 * Created by Wang Haoren
 * --------------------------------
 * Declaration of the helper functions that implement fast fourier transformation
 * */

#ifndef CALSTACK_MULT_FFT_HELPER_H_
#define CALSTACK_MULT_FFT_HELPER_H_

#include <complex.h>

#include "numeric_cfg.h"

typedef int64 Uldbyte;

size_t align_p2(const Udbyte *seq1, const Udbyte *seq2, size_t len1, size_t len2);  // Align the length by power of 2

void bitrevtb(unsigned int64 *table, size_t size);  // Calculate the Bit-reversal table

void bit_rev_shuffle(long double complex *val, size_t len, unsigned int64 *table);  // Bit-reversal shuffle

void fourier_transform(size_t len, unsigned int64 *table); // Do the fast-fourier transformation

Uldbyte result_copy(Uldbyte *tar, size_t len); // Copy and fix the result

#endif
