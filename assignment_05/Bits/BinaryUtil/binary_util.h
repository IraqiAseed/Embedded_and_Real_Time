#ifndef BINARY_UTIL_H_
#define BINARY_UTIL_H_


#include<stdio.h>
#include <limits.h>

/**
 * Prints the binary representation of an unsigned char (8 bits).
 *
 * This function prints the binary form of the given value to stdout.
 * The output is always 8 bits (from MSB to LSB).
 *
 * @param x The unsigned char value to be printed in binary format.
 *
 * @return None.
 *
 * @note
 *        - The function does not return a value; it prints directly to stdout.
 *        - Output format example: 00000101 (binary)\n
 **/
void print_binary(unsigned char x);



void print_binary_int(unsigned int x);
#endif // BINARY_UTIL_H_