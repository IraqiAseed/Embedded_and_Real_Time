#ifndef DYNAMICARRAY_H_
#define DYNAMICARRAY_H_

typedef enum DynamicArrayError
{
    DYNAMIC_ARRAY_SUCCESS = 0,
    DYNAMIC_ARRAY_NULL_ERROR,
    DYNAMIC_ARRAY_INNER_ARRAY_NULL_ERROR,
    DYNAMIC_ARRAY_OVERFLOW,
    DYNAMIC_ARRAY_UNDERFLOW
} DynamicArrayError;

typedef struct DynamicArray
{
    int *dArray;
    int dArraySize; // capacity
    int dArrayBlockSize;
    int NumOfElements; // count
} DynamicArray;

DynamicArray *DynamicArrayCreate(int size, int blockSize);
void DynamicArrayDestroy(DynamicArray* dynamicArrayPtr);
DynamicArrayError DynamicArrayInsert(DynamicArray* dynamicArrayPtr, int data);
DynamicArrayError DynamicArrayDelete(DynamicArray* dynamicArrayPtr, int* data);
void PrintDynamicArrayStatus(DynamicArray *dynamicArrayPtr);

#endif // DYNAMICARRAY_H_