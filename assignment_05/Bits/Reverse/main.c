#include"reverse.h"

int main()
{

    
    unsigned int  x = 77;
    unsigned int reversed = 0;

    if (reverse_bits(x, &reversed) == 0)
    {
        printf("x =  ");
        print_binary_int(x);

        printf("reversed =  ");
        print_binary_int(reversed);
        
    }
    else
    {
        printf("FAIL to reverse number");
    }

    return 0;
}