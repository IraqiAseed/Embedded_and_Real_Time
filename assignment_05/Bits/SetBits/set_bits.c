#include "set_bits.h"

int setbits(unsigned char *ch, size_t p, size_t n, unsigned char y)
{

    if (ch == NULL || p < 1 || p > 8 || n < 1 || n > 8 || n > p)
    {
        return -1;
    }

    unsigned char y_bit = 0;
    size_t y_bit_pos = 0;

    for (size_t i = 0; i < n; ++i)
    {
        y_bit = y & 1u;
        y >>= 1;
        y_bit_pos = (p - n ) + i;
        y_bit <<= y_bit_pos ;
        (*ch) = (~(1u << y_bit_pos)) & (*ch);
        (*ch) |= y_bit;
    }

    return 0;
}