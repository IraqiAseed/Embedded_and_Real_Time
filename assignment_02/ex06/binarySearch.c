#include "binarySearch.h"

int binary_search(int arr[], size_t size, int number, int *index)
{
    if (arr == NULL || size == 0 || index == NULL)
    {
        return -1;
    }
    *index = -1;
    int right = (int)size - 1;
    int left = 0;
    int mid = -1;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (number == arr[mid])
        {
            *index = mid;
            return 0;
        }
        else if (number > arr[mid])
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
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
void run_test(int arr[], size_t size, int number, const char *name)
{
    int index = -1;

    printf("================================\n");
    printf("Test: %s\n", name);

    printf("Array: ");
    if (print_array(arr, size) != 0)
    {
        printf("invalid array\n");
        return;
    }

    printf("Search number %d...\n", number);

    if (binary_search(arr, size, number, &index) != 0)
    {
        printf("Number %d not found, index = %d\n", number, index);
        return;
    }

    printf("Number %d found at index %d\n", number, index);
}