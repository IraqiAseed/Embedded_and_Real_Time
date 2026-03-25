#ifndef REVERSE_LOOKUPTABLE_H_
#define REVERSE_LOOKUPTABLE_H_
#include"./../../assignment_05/Bits/BinaryUtil/binary_util.h"





#include<stdio.h>

/**
 * Reverses the bit order of an unsigned char.
 *
 * This function takes an unsigned char and produces a new value where
 * all bits are reversed in order:
 *   - The least significant bit (LSB) becomes the most significant bit (MSB)
 *   - The MSB becomes the LSB
 *
 * The result is stored in the memory location pointed to by `reversed`.
 *
 * @param number The input unsigned char whose bits will be reversed.
 *
 * @param reversed Pointer to an unsigned char where the reversed result will be stored.
 *        Must not be NULL.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (if `reversed` is NULL).
 *
 */
int reverse_8bits(unsigned char number, unsigned char* reversed);


/**
 * Reverses the bit order of a 32-bit unsigned integer using a lookup table.
 *
 * This function computes the bit-reversed value of the input number. Instead of
 * reversing all 32 bits one by one, it uses a precomputed lookup table for all
 * possible 8-bit values (0–255). Each byte of the input is reversed independently
 * and then reassembled in reverse byte order to produce the final result.
 *
 * Algorithm:
 * 1. A static lookup table of size 256 is initialized once. Each entry contains
 *    the bit-reversed value of its index (8-bit reversal).
 * 2. The input number is split into 4 bytes (b0, b1, b2, b3).
 * 3. Each byte is reversed using the lookup table.
 * 4. The reversed bytes are reassembled in reversed order:
 *      reversed = [rev(b0) << 24 | rev(b1) << 16 | rev(b2) << 8 | rev(b3)]
 *
 * @param number The 32-bit unsigned integer whose bits will be reversed.
 *
 * @param reversed Pointer to an unsigned int where the result will be stored.
 *
 * @return
 *          0  on success.  
 *          -1 if the input pointer 'reversed' is NULL or if table initialization fails.
 *
 * @note
 * - The lookup table is initialized only once using a static flag.
 * - This implementation assumes that unsigned int is 32 bits (4 bytes).
 *
 * @warning
 * - Behavior is undefined if 'reversed' is NULL.
 * - For systems where sizeof(unsigned int) != 4, this implementation
 *   must be adapted.
 */
int reverse_bits(unsigned int number, unsigned int *reversed);

#endif // REVERSE_LOOKUPTABLE_H_