#include "bubleSortTiming.h"

int main(void)
{
    
    srand(time(NULL));
    int arr1[60000];
    int arr2[60000];
    for(size_t i = 0; i < 60000; ++i)
    {
        arr1[i] = rand() % 80000;
        arr2[i] = arr1[i];
    }

    
    
    sortAlgorithm sort[] = {
                            {"Bubble Sort with swapMacro", bubleSort_Macro},
                            {"Bubble Sort with swap function", bubleSort}};
  
    run_test(arr1, sizeof(arr1) / sizeof(arr1[0]), "random numbers swap macro", sort[0]);
    run_test(arr2, sizeof(arr2) / sizeof(arr2[0]), "random numbers swap function", sort[1]);
    
   




    return 0;
}