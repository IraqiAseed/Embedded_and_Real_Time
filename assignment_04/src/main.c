#include "../include/DynamicArray.h"
#include <stdio.h>
#include <stdbool.h>

void PrintTestResult(char testName[], int result)
{
    if (result)
    {
        printf("PASS: %s\n", testName);
    }
    else
    {
        printf("FAIL: %s\n", testName);
    }
}

void TestCreate(DynamicArray* d_arr)
{
    printf("    Test Create     \n");
    PrintTestResult("Initial capacity is 5", d_arr->dArraySize == 5);
    PrintTestResult("Initial number of elements is 0", d_arr->NumOfElements == 0);
    PrintDynamicArrayStatus(d_arr);
}

void TestInsertWithoutResize(DynamicArray* d_arr)
{
    printf("     Test Insert without resize     \n");

    PrintTestResult("Insert 2", DynamicArrayInsert(d_arr, 2) == DYNAMIC_ARRAY_SUCCESS);
    PrintTestResult("Insert 4", DynamicArrayInsert(d_arr, 4) == DYNAMIC_ARRAY_SUCCESS);
    PrintTestResult("Insert 6", DynamicArrayInsert(d_arr, 6) == DYNAMIC_ARRAY_SUCCESS);
    PrintTestResult("Insert 8", DynamicArrayInsert(d_arr, 8) == DYNAMIC_ARRAY_SUCCESS);

    PrintTestResult("Number of elements is 4", d_arr->NumOfElements == 4);
    PrintTestResult("Capacity still 5", d_arr->dArraySize == 5);

    PrintDynamicArrayStatus(d_arr);
}

void TestInsertWithResize(DynamicArray* d_arr)
{
    printf("     Test Insert with resize    \n");

    PrintTestResult("Insert 10", DynamicArrayInsert(d_arr, 10) == DYNAMIC_ARRAY_SUCCESS);
    PrintTestResult("Insert 12 - should resize", DynamicArrayInsert(d_arr, 12) == DYNAMIC_ARRAY_SUCCESS);

    PrintTestResult("Capacity after resize is 15", d_arr->dArraySize == 15);
    PrintTestResult("Number of elements is 6", d_arr->NumOfElements == 6);

    PrintDynamicArrayStatus(d_arr);
}

void TestDelete(DynamicArray* d_arr)
{
    int deletedValue = 0;

    printf("     Test Delete    \n");

    PrintTestResult("Delete last element", DynamicArrayDelete(d_arr, &deletedValue) == DYNAMIC_ARRAY_SUCCESS);
    PrintTestResult("Deleted value is 12", deletedValue == 12);
    PrintTestResult("Number of elements is 5", d_arr->NumOfElements == 5);

    PrintDynamicArrayStatus(d_arr);
}

void TestUnderflow(DynamicArray* d_arr)
{
    int deletedValue = 0;

    printf("     Test Underflow     \n");

    while (DynamicArrayDelete(d_arr, &deletedValue) == DYNAMIC_ARRAY_SUCCESS)
    {
    }

    PrintTestResult("Delete from empty array returns UNDERFLOW",
                    DynamicArrayDelete(d_arr, &deletedValue) == DYNAMIC_ARRAY_UNDERFLOW);

    PrintDynamicArrayStatus(d_arr);
}

void TestNullHandling(DynamicArray* d_arr)
{
    int deletedValue = 0;

    printf("     Test NULL handling     \n");

    PrintTestResult("Insert on NULL array",
                    DynamicArrayInsert(NULL, 100) == DYNAMIC_ARRAY_NULL_ERROR);

    PrintTestResult("Delete on NULL array",
                    DynamicArrayDelete(NULL, &deletedValue) == DYNAMIC_ARRAY_NULL_ERROR);

    PrintTestResult("Delete with NULL data pointer",
                    DynamicArrayDelete(d_arr, NULL) == DYNAMIC_ARRAY_NULL_ERROR);
}

int main()
{
    DynamicArray* d_arr = DynamicArrayCreate(5, 10);

    if (d_arr == NULL)
    {
        printf("FAIL: DynamicArrayCreate returned NULL\n");
        return 1;
    }

    TestCreate(d_arr);
    printf("\n");
    TestInsertWithoutResize(d_arr);
    printf("\n");
    TestInsertWithResize(d_arr);
    printf("\n");
    TestDelete(d_arr);
    printf("\n");
    TestUnderflow(d_arr);
    printf("\n");
    TestNullHandling(d_arr);

    DynamicArrayDestroy(d_arr);
    d_arr = NULL;

    PrintTestResult("Pointer set to NULL after destroy", d_arr == NULL);

    return 0;
}