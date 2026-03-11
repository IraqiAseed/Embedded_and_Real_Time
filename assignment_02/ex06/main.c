#include "binarySearch.h"

int main(void)
{
    int arr0[] = {1, 3, 5, 7, 8, 10, 18};
    int arr1[] = {1, 2, 3, 5, 9, 25, 55, 60, 91};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr4[] = {2};
    int arr5[] = {1, 3, 5, 7, 8, 10, 18};

    run_test(arr0, sizeof(arr0) / sizeof(arr0[0]), 5, "search number at index 2");
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), 5, "search number at index 3");
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), 9, "search number at last index");
    run_test(arr3, sizeof(arr3) / sizeof(arr3[0]), 1, "search number first index");
    run_test(arr4, sizeof(arr4) / sizeof(arr4[0]), 2, "search number first index");
    run_test(arr5, sizeof(arr5) / sizeof(arr5[0]), 9, "search number not found");

    return 0;
}