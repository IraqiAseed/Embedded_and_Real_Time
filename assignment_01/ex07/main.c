#include <stdio.h>

/*
The function expects base and check to have the same number of digits.
If lengths differ, it is considered an error and the function returns -1.
*/
int DigitsCheck(int base, int check, int *result)
{
    int tens_counter = 0;
    int once_counter = 0;
    int base_digits_arr[10] = {0};
    int check_digits_arr[10] = {0};
    int base_digit = 0;
    int check_digit = 0;
    int tmp_base = base;

    if (result == NULL || base < 0 || check < 0)
    {
        return -1;
    }

    while (tmp_base)
    {
        base_digit = tmp_base % 10;

        base_digits_arr[base_digit]++;

        if (base_digits_arr[base_digit] > 1)
        {
            return -1;
        }

        tmp_base /= 10;
    }

    while (base && check)
    {
        base_digit = base % 10;

        check_digit = check % 10;

        check_digits_arr[check_digit]++;

        if (check_digits_arr[check_digit] > 1)
        {
            return -1;
        }

        if (base_digit == check_digit)
        {
            tens_counter++;
        }
        else if (base_digits_arr[check_digit])
        {
            once_counter++;
        }

        base /= 10;
        check /= 10;
    }

    if (base != 0 || check != 0)
        return -1;

    *result = tens_counter * 10 + once_counter;
    return 0;
}

int main()
{
    int res;
    if (DigitsCheck(1234, 4321, &res) == 0)
        printf("1234 vs 4321 -> result = %d\n", res);
    else
        printf("1234 vs 4321 -> error\n");

    if (DigitsCheck(2468, 2648, &res) == 0)
        printf("2468 vs 2648 -> result = %d\n", res);
    else
        printf("2468 vs 2648 -> error\n");

    if (DigitsCheck(1234, 9321, &res) == 0)
        printf("1234 vs 9321 -> result = %d\n", res);
    else
        printf("1234 vs 9321 -> error\n");

    if (DigitsCheck(1324, 9321, &res) == 0)
        printf("1324 vs 9321 -> result = %d\n", res);
    else
        printf("1324 vs 9321 -> error\n");

    if (DigitsCheck(1234, 921, &res) == 0)
        printf("1234 vs 921 -> result = %d\n", res);
    else
        printf("1234 vs 921 -> error\n");

    if (DigitsCheck(234, 4321, &res) == 0)
        printf("234 vs 4321 -> result = %d\n", res);
    else
        printf("234 vs 4321 -> error\n");
}
