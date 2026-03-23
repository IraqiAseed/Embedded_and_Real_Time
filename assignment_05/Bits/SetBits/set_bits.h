#ifndef SET_BITS_H_
#define SET_BITS_H_

#include "../BinaryUtil/binary_util.h"
#include <stdio.h>



/**
 * Replaces n bits in an unsigned char with bits from another value.
 *
 * This function sets n bits in the value pointed to by `ch`, starting at position `p`,
 * using the rightmost n bits from the value `y`. All other bits in `ch` remain unchanged.
 *
 * Bit positions are interpreted using 1-based indexing from the right:
 *   - Position 1 = least significant bit (LSB)
 *   - Position 8 = most significant bit (MSB)
 *
 * @param ch Pointer to the target unsigned char whose bits will be modified.
 *           Must not be NULL.
 *
 * @param p The ending position (1-based) of the bit range to modify.
 *
 * @param n Number of bits to replace.
 *          The bits replaced are from positions (p - n + 1) to p.
 *
 * @param y Source value. The rightmost n bits of `y` will be used.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (invalid parameters such as NULL pointer or out-of-range values).
 *
 * @note
 *        - Only the rightmost n bits of `y` are used.
 *        - The function modifies `*ch` in place.
 *        - Internally, bit positions are converted from 1-based to 0-based indexing
 *          for bitwise operations.
 *
 * @example
 *        unsigned char x = 0b10101010; // 170
 *        unsigned char y = 0b10100111; // 167
 *
 *        setbits(&x, 6, 3, y);
 *
 *        // Replaces bits at positions 6,5,4 with rightmost 3 bits of y (111)
 *        // Result:
 *        // x = 0b10111010 (186)
 */

int setbits(unsigned char *ch, size_t p, size_t n, unsigned char y);

#endif // SET_BITS_H_