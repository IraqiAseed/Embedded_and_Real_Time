#include "reorderArray.h"

int array_reorder(int arr[], size_t size, size_t *evens)
{
    if (arr == NULL || size == 0 || evens == NULL)
    {
        return -1;
    }

    size_t evens_pos = 0;
    size_t curr_pos = 0;
    *evens = 0;
    int even = 1;
    
    while (curr_pos < size)
    {
        if (arr[curr_pos] % 2 == 0)
        {
            even = arr[curr_pos]; // save current even

            // shift array by one element to right
            for (size_t j = curr_pos; j > evens_pos; --j)
            {
                arr[j] = arr[j - 1];
            }

            arr[evens_pos] = even; // update even
            evens_pos++;
            (*evens)++;
        }

        curr_pos++;
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

void run_test(int arr[], size_t size, const char *name)
{
    size_t evens = 0;

    printf("================================\n");
    printf("Test: %s\n", name);

    printf("Before: ");
    print_array(arr, size);

    if (array_reorder(arr, size, &evens) != 0)
    {
        printf("array_reorder failed\n");
        return;
    }

    printf("After : ");
    print_array(arr, size);

    printf("Number of evens = %zu\n", evens);
}