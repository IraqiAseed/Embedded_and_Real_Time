#include "bubleSortTiming.h"


#define SWAP(a,b) { int (tmp)=(a);(a)=(b);(b)=(tmp); }

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int bubleSort(int arr[], size_t size)
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
            if (arr[j] > arr[j + 1])
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
int bubleSort_Macro(int arr[], size_t size)
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
            if (arr[j] > arr[j + 1])
            {
                SWAP(arr[j], arr[j + 1]);
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

void run_test(int arr[], size_t size, const char *name, sortAlgorithm algo)
{

    printf("================================\n");
    printf("Test: %s\n", name);

    printf("Before: ");
    print_array(arr, size);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (algo.sortptr(arr, size) != 0)
    {
        printf("array buble sort failed\n");
        return;
    }


    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("After : ");
    print_array(arr, size);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%s Time taken: %f seconds\n", algo.name, time_taken);
    
}
