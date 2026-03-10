#include "bubleSort.h"

int main(void)
{
    int arr0[] = {1, 3, 5, 2, 7, 9, 4};
    int arr1[] = {1, 3, 2, 5, 4, 7, 6, 9, 8};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr3[] = {2};

    run_test(arr0, sizeof(arr0) / sizeof(arr0[0]), "mixed numbers 1");
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), "mixed numbers 2");
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), "sorted numbers");
    run_test(arr3, sizeof(arr3) / sizeof(arr3[0]), "single number");

    return 0;
}