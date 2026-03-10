#include "occurrence.h"
void test_array(const int arr[], size_t size)
{
    int value = 0;
    int count = 0;

    if (max_occurrence(arr, size, &value, &count) != 0)
    {
        printf("Error: max_occurrence failed\n\n");
        return;
    }

    printf("Array: ");
    for (size_t i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }

    printf("\nMost frequent value: %d\n", value);
    printf("Occurrences: %d\n\n", count);
}

int main(void)
{
    int arr0[] = {1, 2, 1, 4, 5, 6, 6, 8, 6, 10};
    int arr1[] = {1, 2, 3, 2, 4, 2, 5, 3, 3, 3};
    int arr2[] = {5, 5, 5, 5, 5};
    int arr3[] = {1, 2, 3, 4, 5};
    int arr4[] = {7};

    test_array(arr0, sizeof(arr0) / sizeof(arr0[0]));
    test_array(arr1, sizeof(arr1) / sizeof(arr1[0]));
    test_array(arr2, sizeof(arr2) / sizeof(arr2[0]));
    test_array(arr3, sizeof(arr3) / sizeof(arr3[0]));
    test_array(arr4, sizeof(arr4) / sizeof(arr4[0]));

    return 0;
}
