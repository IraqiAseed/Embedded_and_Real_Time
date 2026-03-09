#include"average.h"

float average(int arr[],size_t size)
{
    
    int sum = 0;

    if (arr == NULL || size == 0)
    {
        return 0.0f;
    }
    
    
    for(int i = 0 ; i < size ; ++i)
    {
        sum +=arr[i];
    }

    return (float)sum/size;
}

