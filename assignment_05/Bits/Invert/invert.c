#include"invert.h"

int Invert_bits(unsigned char ch,unsigned char* inverted)
{
    if(inverted == NULL)
    {
        return -1;
    }
    
    unsigned char bit = 0;
    *inverted = 0;
    
    for(int i=0 ; i < CHAR_BIT ; ++i)
    {
        bit = (ch & 1) ^ 1;
        (*inverted) |= (bit << i);
        ch >>= 1;
    }

    return 0;
}