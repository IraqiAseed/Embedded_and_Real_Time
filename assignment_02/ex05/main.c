#include "zerosOnes.h"

int main(void)
{
    int arr0[] = {1, 0, 1, 0, 1, 0, 1};
    int arr1[] = {1, 0, 0, 0, 1, 0, 0, 0, 0};
    int arr2[] = {1, 1, 1, 1, 1};
    int arr3[] = {0, 0, 0, 0, 0};
    int arr4[] = {0};
    int arr5[] = {1};
    int arr6[] = {0, 1};
    int arr7[] = {1, 0};
    int arr8[] = {1, 0, 6};

    //TEST 1 - sort_zeros_ones_by_twoPointers
    run_test(arr0, sizeof(arr0) / sizeof(arr0[0]), true, "mixed numbers");
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), true, "mixed numbers");
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), true, "all ones");
    run_test(arr3, sizeof(arr3) / sizeof(arr3[0]), true, "all zeros");
    run_test(arr4, sizeof(arr4) / sizeof(arr4[0]), true, "single zero");
    run_test(arr5, sizeof(arr5) / sizeof(arr5[0]), true, "single one");
    run_test(arr6, sizeof(arr6) / sizeof(arr6[0]), true, "two elements even zero one");
    run_test(arr7, sizeof(arr7) / sizeof(arr7[0]), true, "two elements one zero");
    run_test(arr8, sizeof(arr8) / sizeof(arr8[0]), true, "invalid values");

    //TEST 2: sort_zeros_ones_by_count
    run_test(arr0, sizeof(arr0) / sizeof(arr0[0]), false, "mixed numbers");
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), false, "mixed numbers");
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), false, "all ones");
    run_test(arr3, sizeof(arr3) / sizeof(arr3[0]), false, "all zeros");
    run_test(arr4, sizeof(arr4) / sizeof(arr4[0]), false, "single zero");
    run_test(arr5, sizeof(arr5) / sizeof(arr5[0]), false, "single one");
    run_test(arr6, sizeof(arr6) / sizeof(arr6[0]), false, "two elements even zero one");
    run_test(arr7, sizeof(arr7) / sizeof(arr7[0]), false, "two elements one zero");
    run_test(arr8, sizeof(arr8) / sizeof(arr8[0]), false, "invalid values");

    return 0;
}