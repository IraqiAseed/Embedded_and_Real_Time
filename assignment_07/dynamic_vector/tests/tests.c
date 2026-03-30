#include "tests.h"

void PrintResult(const char *testName, int condition)
{
    if (condition)
    {
        printf("[PASS] %s\n", testName);
    }
    else
    {
        printf("[FAIL] %s\n", testName);
    }
}

int PrintInt(void *element, size_t index, void *context)
{
    (void)context;
    if (element == NULL)
    {
        printf("Index %zu = NULL\n", index);
    }
    else
    {
        printf("Index %zu = %d\n", index, *(int *)element);
        return 1;
    }

    return 1;
}

int SumInt(void *element, size_t index, void *context)
{
    (void)index;
    if (element != NULL && context != NULL)
    {
        int *sum = (int *)context;
        *sum += *(int *)element;
    }
    return 1;
}

int StopAtIndex2(void *element, size_t index, void *context)
{
    (void)element;
    (void)context;
    if (index == 2)
    {
        return 0; // stop iteration at index 2
    }

    return 1;
}

void DestroyInt(void *element)
{
    if(element != NULL)
    {
        free(element);
    }
    
}
