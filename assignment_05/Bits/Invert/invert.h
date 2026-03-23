#ifndef INVERT_H_
#define INVERT_H_

#include"./../BinaryUtil/binary_util.h"
#include<stdio.h>


/**
 * Inverts (flips) all bits of an unsigned char.
 *
 * This function takes an unsigned char value and produces a new value
 * where each bit is flipped:
 *   0 → 1
 *   1 → 0
 *
 * The result is stored in the memory location pointed to by `inverted`.
 *
 * @param ch The input unsigned char whose bits will be inverted.
 *
 * @param inverted Pointer to an unsigned char where the inverted result will be stored.
 *        Must not be NULL.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (if `inverted` is NULL).
 */
int Invert_bits(unsigned char ch,unsigned char* inverted);

#endif // INVERT_H_