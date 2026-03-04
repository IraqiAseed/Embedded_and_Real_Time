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
int IsPalindrom(int num)
{
    int reversed = 0;

    if (ReverseInteger(num, &reversed) != REV_SUCCESS)
        return 0;

    return reversed == num;
}

void Test(int num)
{
    printf("Number: %d -> Palindrome: %d\n", num, IsPalindrom(num));
}

int main()
{
    printf("---- Basic Tests ----\n");
    Test(121);
    Test(12321);
    Test(11);
    Test(1);

    printf("\n---- Not Palindrome ----\n");
    Test(123);
    Test(12345);
    Test(12);

    printf("\n---- Ending with zero ----\n");
    Test(10);
    Test(100);
    Test(12021);

    printf("\n---- Zero ----\n");
    Test(0);

    printf("\n---- Negative numbers ----\n");
    Test(-121);
    Test(-12321);
    Test(-10);

    printf("\n---- Large numbers ----\n");
    Test(1534236469); 
    Test(2147447412);  
    Test(2147483647);  

    return 0;
}