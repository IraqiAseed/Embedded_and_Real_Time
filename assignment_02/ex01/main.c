#include "average.h"

int main()
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float arr_average = average(arr, sizeof(arr) / sizeof(arr[0]));

    printf("Average of the array: %.2f\n", arr_average);
    
    return 0;
}