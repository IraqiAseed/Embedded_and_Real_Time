#include "set_bits.h"

int main()
{
    unsigned char x = 0b10101010;
    unsigned char y = 0b10100111;
    size_t n = 3;
    size_t p = 6;

    printf("x =  ");
    print_binary(x);

    if (setbits(&x, p, n, y) == 0)
    {
        printf("y =  ");
        print_binary(y);

        printf("setbits n=%ld , p=%ld gives x = ", n, p);
        print_binary(x);
    }
    else
    {
        printf("FAIL to set bits in the binary number");
    }

    return 0;
}