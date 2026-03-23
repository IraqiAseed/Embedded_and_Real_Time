#include "invert.h"

int main()
{
    unsigned char x = 0b11101010;
    unsigned char y = 0;

    if (Invert_bits(x, &y) == 0)
    {
        printf("x =  ");
        print_binary(x);

        printf("x inverted =  ");
        print_binary(y);
    }
    else
    {
        printf("FAIL to invert binary number");
    }

    return 0;
}