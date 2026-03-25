#include "sort.h"

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int bigger(int a, int b)
{
    return a > b;
}

int smaller(int a, int b)
{
    return a < b;
}
int even_first(int a, int b)
{
    if ((a % 2 == 0) && (b % 2 != 0))
        return 0;

    if ((a % 2 != 0) && (b % 2 == 0))
        return 1; 

    return a > b; 
}
int abs_compare(int a, int b)
{
    int abs_a = (a < 0) ? -a : a;
    int abs_b = (b < 0) ? -b : b;

    return abs_a > abs_b;
}
int print_array(int arr[], size_t size)
{
    if (arr == NULL || size == 0)
    {
        return -1;
    }

    for (size_t i = 0; i < size; ++i)
    {
        printf("%d\t", arr[i]);
    }
    putchar('\n');
    return 0;
}
void run_test(int arr[], size_t size, const char *name, int (*compare)(int a, int b))
{

    printf("================================\n");
    printf("Test: %s\n", name);

    printf("Before: ");
    print_array(arr, size);

    if (bubleSort_with_creteria(arr, size, compare) != 0)
    {
        printf("array buble sort failed\n");
        return;
    }

    printf("After : ");
    print_array(arr, size);
}

int bubleSort_with_creteria(int arr[], size_t size, int (*compare)(int a, int b))
{
    int swaped = 0;

    if (arr == NULL || size == 0)
    {
        return -1;
    }

    for (size_t i = 0; i < size - 1; ++i)
    {
        swaped = 0;
        for (size_t j = 0; j < size - 1 - i; ++j)
        {
            if (compare(arr[j], arr[j + 1]) > 0)
            {
                swap(&arr[j], &arr[j + 1]);
                swaped = 1;
            }
        }
        if (swaped == 0)
        {
            break;
        }
    }
    return 0;
}