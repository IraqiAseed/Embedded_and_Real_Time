#ifndef ROTATE_H_
#define ROTATE_H_

#include"../BinaryUtil/binary_util.h"
#include<stdio.h>


/** Rotates the bits of an unsigned char to the right.
 *
 * This function performs a circular right rotation (bitwise rotation)
 * on an 8-bit unsigned value. Bits shifted out from the least significant
 * position (LSB) are wrapped around to the most significant position (MSB).
 *
 * @param ch The input value whose bits will be rotated.
 *
 * @param pos Number of positions to rotate to the right.
 *            If pos >= 8, it will be reduced modulo 8.
 *
 * @param rotated Pointer to an unsigned char where the rotated result will be stored. Must not be NULL.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (if rotated is NULL).
 *
 * @note
 *        - Rotation is performed on 8 bits (unsigned char).
 *        - Uses iterative shifting with wrap-around.
 *
 *
 * @example
 *        unsigned char x = 0b10110001;
 *        unsigned char result;
 *
 *        rotate_right_bits(x, 1, &result);
 *        // result = 0b11011000
 */

int rotate_right_bits(unsigned char ch, size_t pos, unsigned char* rotated);
int rotate_right_bits_without_loop(unsigned char ch, size_t pos, unsigned char* rotated);

#endif // ROTATE_H_