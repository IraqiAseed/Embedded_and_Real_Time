#include <stdio.h>
#include "reorderArray.h"
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

int main(void)
{
    int arr0[] = {1, 3, 5, 2, 7, 9, 4};
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {2, 4, 6, 8, 10};
    int arr3[] = {1, 3, 5, 7, 9};
    int arr4[] = {1, 3, 2, 5, 4, 7, 6, 9, 8};
    int arr5[] = {2};
    int arr6[] = {1};
    int arr7[] = {2, 1};
    int arr8[] = {1, 2};
   

    run_test(arr0, sizeof(arr0) / sizeof(arr0[0]), "mixed numbers");
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), "mixed numbers");
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), "all even");
    run_test(arr3, sizeof(arr3) / sizeof(arr3[0]), "all odd");
    run_test(arr4, sizeof(arr4) / sizeof(arr4[0]), "mixed stable test");
    run_test(arr5, sizeof(arr5) / sizeof(arr5[0]), "single even");
    run_test(arr6, sizeof(arr6) / sizeof(arr6[0]), "single odd");
    run_test(arr7, sizeof(arr7) / sizeof(arr7[0]), "two elements even odd");
    run_test(arr8, sizeof(arr8) / sizeof(arr8[0]), "two elements odd even");

    return 0;
}