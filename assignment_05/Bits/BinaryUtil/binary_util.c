#include "binary_util.h"
void print_binary(unsigned char x)
{
    for (int i = CHAR_BIT - 1; i >= 0; --i)
    {
        printf("%d", (x >> i) & 1);
    }
    printf(" (binary)\n");
}


void print_binary_int(unsigned int x)
{
    int bits = sizeof(unsigned int) * CHAR_BIT;
    
    for (int i = bits - 1; i >= 0; --i)
    {
        printf("%d", (x >> i) & 1);
    }
    printf(" (binary)\n");
}