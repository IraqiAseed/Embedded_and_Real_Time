#include "reverse.h"

int reverse_bits(unsigned int number, unsigned int* reversed)
{
    if(reversed == NULL)
    {
        return -1;
    }
    
    int length = sizeof(int) * CHAR_BIT;
    *reversed = 0;
    int bit = 0;

    for(int i=0; i<length ; ++i)
    {
        *reversed <<= 1;
        bit = number & 1;
        *reversed |= bit;
        number >>= 1; 
    }
    
    return 0;
}