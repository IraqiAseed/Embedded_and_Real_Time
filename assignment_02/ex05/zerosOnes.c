#include "zerosOnes.h"

int sort_zeros_ones_by_count(int arr[], size_t size)
{
    if (arr == NULL || size == 0)
    {
        return -1;
    }

    int ones = 0;

    for (size_t i = 0; i < size; ++i)
    {
        if (arr[i] != 0 && arr[i] != 1)
        {
            return -1;
        }
        ones += arr[i];
    }

    int zeros = size - ones;

    for (size_t i = 0; i < zeros; ++i)
    {
        arr[i] = 0;
    }

    for (size_t i = zeros; i < size; ++i)
    {
        arr[i] = 1;
    }

    return 0;
}

int sort_zeros_ones_by_twoPointers(int arr[], size_t size)
{
    if (arr == NULL || size == 0)
    {
        return -1;
    }

    // validate input
    for (size_t i = 0; i < size; ++i)
    {
        if (arr[i] != 0 && arr[i] != 1)
        {
            return -1;
        }
    }

    size_t left = 0;
    size_t right = size - 1;

    while (left < right)
    {
        while (left < right && arr[left] == 0)
        {
            left++;
        }

        while (left < right && arr[right] == 1)
        {
            right--;
        }

        if (left < right)
        {
            swap(&arr[left], &arr[right]);
            left++;
            right--;
        }
    }

    return 0;
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
void run_test(int arr[], size_t size, bool use_two_pointers, const char *name)
{

    printf("================================\n");
    printf("Test: %s\n", name);

    printf("Before: ");
    print_array(arr, size);

    if (use_two_pointers && sort_zeros_ones_by_twoPointers(arr, size) != 0)
    {
        printf("array sort failed\n");
        return;
    }
    else if (!use_two_pointers && sort_zeros_ones_by_count(arr, size) != 0)
    {
        printf("array sort failed\n");
        return;
    }

    printf("After : ");
    print_array(arr, size);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}