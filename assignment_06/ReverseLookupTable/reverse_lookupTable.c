#include "reverse_lookupTable.h"
#include <limits.h>

int reverse_8bits(unsigned char number, unsigned char *reversed)
{
    if (reversed == NULL)
    {
        return -1;
    }

    *reversed = 0;

    for (int i = 0; i < 8; ++i)
    {
        *reversed <<= 1;
        *reversed |= (number & 1);
        number >>= 1;
    }

    return 0;
}

int reverse_bits(unsigned int number, unsigned int *reversed)
{
    if (reversed == NULL)
    {
        return -1;
    }

    static int flag = 0;
    static unsigned char table[256];
    unsigned char rev = 0;

    if (flag == 0)
    {
        for (int i = 0; i < 256; ++i)
        {
            if (reverse_8bits((unsigned char)i, &rev) != 0)
            {
                return -1;
            }

            table[i] = rev;
        }

        flag = 1;
    }

    unsigned int b0 = (number >> 0)  & 0xFF;
    unsigned int b1 = (number >> 8)  & 0xFF;
    unsigned int b2 = (number >> 16) & 0xFF;
    unsigned int b3 = (number >> 24) & 0xFF;

    *reversed =
        ((unsigned int)table[b0] << 24) |
        ((unsigned int)table[b1] << 16) |
        ((unsigned int)table[b2] << 8)  |
        ((unsigned int)table[b3] << 0);

    return 0;
}