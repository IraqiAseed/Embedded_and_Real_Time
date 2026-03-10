#include "occurrence.h"
int max_occurrence(const int arr[], size_t size, int *max_occurrence_value, int *occurrences)
{

    if (arr == NULL || size == 0 || max_occurrence_value == NULL || occurrences == NULL)
    {
        return -1;
    }

    int counter = 0;
    int max_counter = 0;
    int seen = 0;

    for (size_t i = 0; i < size; ++i)
    {
        seen = 0;

        // Check whether this value was already processed
        for (size_t j = 0; j < i; ++j)
        {
            if (arr[i] == arr[j])
            {
                seen = 1;
                break;
            }
        }

        if (seen == 1)
        {
            continue;
        }

        // Count occurrences of arr[i] in the whole array
        counter = 0;

        for (size_t j = 0; j < size; ++j)
        {
            if (arr[i] == arr[j])
            {
                counter++;
            }
        }

        if (counter > max_counter)
        {
            max_counter = counter;
            *max_occurrence_value = arr[i];
            *occurrences = counter;
        }
    }

    return 0;
}
