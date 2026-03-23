#include"rotate.h"

int main()
{
    unsigned char x = 0b10100111;
    unsigned char y = 0;
    int rotations = 3;
    
    if (rotate_right_bits(x,rotations, &y) == 0)
    {
        printf("x =  ");
        print_binary(x);

        printf("x rotated by %d =  ",rotations);
        print_binary(y);
    }
    else
    {
        printf("FAIL to rotate binary number");
    }

    return 0;
}