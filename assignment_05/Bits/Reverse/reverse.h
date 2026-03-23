#ifndef REVERSE_H_
#define REVERSE_H_

#include"../BinaryUtil/binary_util.h"
#include<stdio.h>

/**
 * Reverses the bit order of an unsigned integer.
 *
 * This function takes an unsigned integer and produces a new value where
 * all bits are reversed in order:
 *   - The least significant bit (LSB) becomes the most significant bit (MSB)
 *   - The MSB becomes the LSB
 *
 * The result is stored in the memory location pointed to by `reversed`.
 *
 * @param number The input unsigned integer whose bits will be reversed.
 *
 * @param reversed Pointer to an unsigned integer where the reversed result will be stored.
 *        Must not be NULL.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (if `reversed` is NULL).
 *
 * @note
 *        - The operation is performed on all bits of `unsigned int`.
 *        - The bit width depends on the system (commonly 32 bits).
 *        - Logical right shift is used (since the type is unsigned).
 *
 */
int reverse_bits(unsigned int number, unsigned int* reversed);

#endif // REVERSE_H_