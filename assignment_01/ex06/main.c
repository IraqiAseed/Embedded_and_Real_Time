#include <stdio.h>
#include <limits.h>

typedef enum
{
    REV_SUCCESS = 0,
    REV_FAILED = 1,
    REV_OVERFLOW = 2
} RevStatus;

RevStatus ReverseInteger(int a, int *result)
{
    if (!result)
        return REV_FAILED;

    *result = 0;

    while (a)
    {
        int digit = a % 10;

        if (*result > INT_MAX / 10 || (*result == INT_MAX / 10 && digit > INT_MAX % 10))
            return REV_OVERFLOW;

        if (*result < INT_MIN / 10 || (*result == INT_MIN / 10 && digit < INT_MIN % 10))
            return REV_OVERFLOW;
        *result = (*result) * 10 + digit;
        a /= 10;
    }
    return REV_SUCCESS;
}

int main()
{
    int num = 12345;
    int reversed = 0;

    RevStatus status = ReverseInteger(num, &reversed);

    if (status == REV_SUCCESS)
    {
        printf("Original: %d\n", num);
        printf("Reversed: %d\n", reversed);
    }
    else if (status == REV_OVERFLOW)
    {
        printf("Overflow\n");
    }
    else
    {
        printf("Error\n");
    }

    return 0;
}