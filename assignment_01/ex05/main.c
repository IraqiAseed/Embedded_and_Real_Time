#include <stdio.h>
int IsPrime(int num)
{
    if (num <= 1)
        return 0;

    if (num == 2)
        return 1;

    if (num % 2 == 0)
        return 0;

    for (int i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
            return 0;
    }

    return 1;
}

int main()
{
    printf("2  -> %d\n", IsPrime(2));
    printf("3  -> %d\n", IsPrime(3));
    printf("4  -> %d\n", IsPrime(4));
    printf("9  -> %d\n", IsPrime(9));
    printf("17 -> %d\n", IsPrime(17));

    return 0;
}