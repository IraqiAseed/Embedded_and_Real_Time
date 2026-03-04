#include <stdio.h>

int IsAscending(int num)
{
    int prev = 10;
    int curr;

    if (num < 0)
    {
        num *= -1;
    }

    while (num)
    {
        curr = num % 10;

        if (curr > prev)
        {
            return 0;
        }

        prev = curr;
        num /= 10;
    }

    return 1;
}

int main()
{
    printf("1234  -> %d\n", IsAscending(1234));
    printf("1324  -> %d\n", IsAscending(1324));
    printf("1000  -> %d\n", IsAscending(1000));
    printf("1     -> %d\n", IsAscending(1));
    printf("0     -> %d\n", IsAscending(0));
    printf("-1234 -> %d\n", IsAscending(-1234));

    return 0;
}