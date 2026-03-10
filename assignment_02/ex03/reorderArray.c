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