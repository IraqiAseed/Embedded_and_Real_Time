#include"rotate.h"


int rotate_right_bits(unsigned char ch, size_t pos, unsigned char *rotated)
{
    if(rotated == NULL)
    {
        return -1;
    }

    unsigned char bit = 0;
    pos = pos % CHAR_BIT; 

    for(size_t i=0; i<pos; ++i)
    {
      bit = ch & 1;
      ch >>= 1;
      ch |= (bit<<(CHAR_BIT -1));
    }

    *rotated = ch;

    return 0;
}

int rotate_right_bits_without_loop(unsigned char ch, size_t pos, unsigned char *rotated)
{
    if (rotated == NULL)
    {
        return -1;
    }

    pos %= 8;

    *rotated = (unsigned char)((ch >> pos) | (ch << (8 - pos)));

    return 0;
}