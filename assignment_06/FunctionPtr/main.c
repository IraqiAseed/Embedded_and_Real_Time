#include "sort.h"

typedef int (*CompareFunc)(int, int);

typedef struct CompareEntry
{
    const char *name;
    CompareFunc func;
} CompareEntry;

void copy_array(int dst[], const int src[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

int main(void)
{
    int original[] = {1, -3, 5, 2, 7, 9, 4};
    size_t size = sizeof(original) / sizeof(original[0]);

    CompareEntry comparators[] =
    {
        {"Descending order", bigger},
        {"Ascending order", smaller},
        {"Absolute value order", abs_compare},
        {"Even numbers first", even_first}
    };

    size_t num_of_comparators = sizeof(comparators) / sizeof(comparators[0]);

    printf("Original array before any sort:\n");
    print_array(original, size);
    printf("\n");

    for (size_t i = 0; i < num_of_comparators; i++)
    {
        int arr_copy[size];

        copy_array(arr_copy, original, size);

        run_test(arr_copy, size, comparators[i].name, comparators[i].func);
    }

    printf("\n");

    return 0;
}