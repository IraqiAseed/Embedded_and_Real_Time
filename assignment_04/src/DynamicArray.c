#include "../include/DynamicArray.h"
#include <stdio.h>
#include <stdlib.h>

DynamicArray *DynamicArrayCreate(int size, int blockSize)
{
    if (size <= 0 || blockSize <= 0)
    {
        return NULL;
    }

    DynamicArray *dynamicArrayPtr = (DynamicArray *)malloc(sizeof(DynamicArray));

    if (dynamicArrayPtr == NULL)
    {
        return NULL;
    }

    dynamicArrayPtr->dArray = (int *)malloc(sizeof(int) * size);

    if (dynamicArrayPtr->dArray == NULL)
    {
        free(dynamicArrayPtr);
        return NULL;
    }

    dynamicArrayPtr->dArraySize = size;
    dynamicArrayPtr->dArrayBlockSize = blockSize;
    dynamicArrayPtr->NumOfElements = 0;

    return dynamicArrayPtr;
}
void DynamicArrayDestroy(DynamicArray *dynamicArrayPtr)
{
    if (dynamicArrayPtr == NULL)
    {
        return;
    }
    free(dynamicArrayPtr->dArray);
    free(dynamicArrayPtr);
}

DynamicArrayError DynamicArrayInsert(DynamicArray *dynamicArrayPtr, int data)
{

    if (dynamicArrayPtr == NULL)
    {
        return DYNAMIC_ARRAY_NULL_ERROR;
    }

    if (dynamicArrayPtr->dArraySize == dynamicArrayPtr->NumOfElements)
    {
        int *ptrNewDynamicArray = (int *)realloc(
            dynamicArrayPtr->dArray,
            sizeof(int) * (dynamicArrayPtr->dArraySize + dynamicArrayPtr->dArrayBlockSize));

        if (ptrNewDynamicArray == NULL)
        {
            return DYNAMIC_ARRAY_INNER_ARRAY_NULL_ERROR;
        }

        dynamicArrayPtr->dArray = ptrNewDynamicArray;
        dynamicArrayPtr->dArraySize += dynamicArrayPtr->dArrayBlockSize;
    }

    dynamicArrayPtr->dArray[dynamicArrayPtr->NumOfElements] = data;
    dynamicArrayPtr->NumOfElements++;

    return DYNAMIC_ARRAY_SUCCESS;
}

DynamicArrayError DynamicArrayDelete(DynamicArray *dynamicArrayPtr, int *data)
{
    if (dynamicArrayPtr == NULL || data == NULL)
    {
        return DYNAMIC_ARRAY_NULL_ERROR;
    }

    if (dynamicArrayPtr->NumOfElements == 0)
    {
        return DYNAMIC_ARRAY_UNDERFLOW;
    }

    int index_to_delete = dynamicArrayPtr->NumOfElements - 1;
    *data = dynamicArrayPtr->dArray[index_to_delete]; // data = last element
    dynamicArrayPtr->NumOfElements--;

    return DYNAMIC_ARRAY_SUCCESS;
}

void PrintDynamicArrayStatus(DynamicArray *DynamicArrayPtr)
{
    printf("     Array Status     \n");

    if (DynamicArrayPtr == NULL)
    {
        printf("DynamicArray is NULL\n");
        return;
    }

    printf("array size is =  %d\n", DynamicArrayPtr->dArraySize);
    printf("array number of elements =  %d\n", DynamicArrayPtr->NumOfElements);

    for (int i = 0; i < DynamicArrayPtr->NumOfElements; ++i)
    {
        printf("array[%d]=  %d\n", i, DynamicArrayPtr->dArray[i]);
    }

    printf("\t\t\t\t\t\n");
}