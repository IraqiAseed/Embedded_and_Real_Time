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

int RunVectorTests()
{
// Test 1: Create Vectors
    printf("========== VECTOR TESTS START ==========\n\n");

    Vector *v1 = VectorCreate(0, 0);
    PrintResult("Create(0,0) should return NULL", v1 == NULL);

    Vector *fixedVec = VectorCreate(2, 0);
    PrintResult("Create fixed-size vector", fixedVec != NULL);

    Vector *growVec = VectorCreate(0, 2);
    PrintResult("Create growable vector with initialCapacity=0", growVec != NULL);

    if (fixedVec != NULL)
    {
        PrintResult("Initial fixedVec size == 0", VectorSize(fixedVec) == 0);
        PrintResult("Initial fixedVec capacity == 2", VectorCapacity(fixedVec) == 2);
    }

    if (growVec != NULL)
    {
        PrintResult("Initial growVec size == 0", VectorSize(growVec) == 0);
        PrintResult("Initial growVec capacity == 0", VectorCapacity(growVec) == 0);
    }

    printf("\n");

    // Test 2: Append elements to growVec and check size/capacity
    for (int i = 1; i <= 5; ++i)
    {
        int *p = malloc(sizeof(int));
        if (p == NULL)
        {
            printf("malloc failed in test setup\n");
            return 1;
        }

        *p = i * 10;

        VectorResult r = VectorAppend(growVec, p);
        PrintResult("Append to growVec", r == VECTOR_SUCCESS);
    }

    PrintResult("growVec size after 5 appends == 5", VectorSize(growVec) == 5);
    PrintResult("growVec capacity after growth >= 5", VectorCapacity(growVec) >= 5);

    printf("growVec elements after append:\n");

    // use for each to print elements
    VectorForEach(growVec, PrintInt, NULL);
    printf("\n");

    // Test 3: Append to fixed-size full vector
    if (fixedVec != NULL)
    {
        // stack variable to append, we will not free it since it's not heap allocated,
        // and it's just for testing append failure on fixed size vector when it's full.
        int a = 11;
        int b = 22;
        int c = 33;

        PrintResult("Append first to fixedVec",
                    VectorAppend(fixedVec, &a) == VECTOR_SUCCESS);

        PrintResult("Append second to fixedVec",
                    VectorAppend(fixedVec, &b) == VECTOR_SUCCESS);

        PrintResult("Append third to fixed-size full vector should fail",
                    VectorAppend(fixedVec, &c) != VECTOR_SUCCESS);
    }

    printf("\n");

    // Test 4: Get
    {
        size_t beforeSize = VectorSize(growVec);
        size_t beforeCap = VectorCapacity(growVec);

        void *out = NULL;
        VectorResult r = VectorGet(growVec, 2, &out);

        PrintResult("Get index 2 from growVec", r == VECTOR_SUCCESS);
        PrintResult("Value at index 2 should be 30", out != NULL && *(int *)out == 30);
        PrintResult("Get should not change size", VectorSize(growVec) == beforeSize);
        PrintResult("Get should not change capacity", VectorCapacity(growVec) == beforeCap);

        r = VectorGet(growVec, 100, &out);
        PrintResult("Get out of bounds should fail", r != VECTOR_SUCCESS);
        PrintResult("Failed Get should not change size", VectorSize(growVec) == beforeSize);
        PrintResult("Failed Get should not change capacity", VectorCapacity(growVec) == beforeCap);
    }

    printf("\n");

    // Test 5: Set */
    {
        int *newVal = malloc(sizeof(int));
        if (newVal == NULL)
        {
            printf("malloc failed in test setup\n");
            return 1;
        }
        *newVal = 999;

        VectorResult r = VectorSet(growVec, 1, newVal);
        PrintResult("Set index 1 in growVec", r == VECTOR_SUCCESS);

        void *out = NULL;
        r = VectorGet(growVec, 1, &out);
        PrintResult("Get index 1 after set", r == VECTOR_SUCCESS);
        PrintResult("Value at index 1 should be 999", out != NULL && *(int *)out == 999);
    }

    printf("\ngrowVec elements after set:\n");
    VectorForEach(growVec, PrintInt, NULL);
    printf("\n");

    // Test 6: ForEach + Context
    {
        int sum = 0;
        size_t calls = VectorForEach(growVec, SumInt, &sum);
        PrintResult("ForEach sum callback called", calls == VectorSize(growVec));
        PrintResult("ForEach sum value check", sum == (10 + 999 + 30 + 40 + 50)); // 10 + 999 + 30 + 40 + 50 = 1129
        printf("Sum of elements in growVec = %d\n", sum);
    }

    {
        // check calls number when stop callback is used
        size_t calls = VectorForEach(growVec, StopAtIndex2, NULL);
        PrintResult("ForEach stop callback should stop after 3 calls", calls == 3);
    }

    printf("\n");

    // Test 7: Remove + Shrink
    {
        size_t beforeSize = VectorSize(growVec);
        size_t beforeCap = VectorCapacity(growVec);

        void *removed = NULL;
        VectorResult r = VectorRemove(growVec, &removed);
        PrintResult("Remove from growVec", r == VECTOR_SUCCESS);

        if (r == VECTOR_SUCCESS && removed != NULL)
        {
            printf("Removed value = %d\n", *(int *)removed);
            free(removed);
        }

        PrintResult("Size decreased by 1", VectorSize(growVec) == beforeSize - 1);

        printf("Capacity before remove = %zu, after remove = %zu\n",
               beforeCap, VectorCapacity(growVec));
    }

    printf("growVec elements after remove:\n");
    VectorForEach(growVec, PrintInt, NULL);
    printf("\n");

    // Test 8: Remove all elements to check shrink and empty state
    while (VectorSize(growVec) > 0)
    {
        void *removed = NULL;
        if (VectorRemove(growVec, &removed) == VECTOR_SUCCESS)
        {
            free(removed);
        }
    }

    PrintResult("growVec size after removing all == 0", VectorSize(growVec) == 0);

    {
        void *removed = NULL;
        VectorResult r = VectorRemove(growVec, &removed);
        PrintResult("Remove from empty vector should fail", r != VECTOR_SUCCESS);
    }

    printf("\n");

    // Cleanup - Destroy vectors
    // Test 9: Destroy 1, because of removed elements we send NULL as element destroy function,
    // if we had not removed the elements we would need to send a function that free the int pointers.
    VectorDestroy(&growVec, NULL);
    PrintResult("Destroy growVec sets pointer to NULL", growVec == NULL);

    VectorDestroy(&fixedVec, NULL);
    PrintResult("Destroy fixedVec sets pointer to NULL", fixedVec == NULL);

    //Test 10: Destroy with callback to free heap elements
    {
        Vector *vecDestroy = VectorCreate(2, 2);
        PrintResult("Create vecDestroy", vecDestroy != NULL);

        if (vecDestroy != NULL)
        {
            for (int i = 1; i <= 3; ++i)
            {
                int *p = malloc(sizeof(int));
                if (p == NULL)
                {
                    printf("malloc failed in Test 10\n");
                    VectorDestroy(&vecDestroy, NULL);
                    return 1;
                }

                *p = i * 100;

                PrintResult("Append heap item to vecDestroy",
                            VectorAppend(vecDestroy, p) == VECTOR_SUCCESS);
            }

            printf("vecDestroy before destroy:\n");
            VectorForEach(vecDestroy, PrintInt, NULL);

            VectorDestroy(&vecDestroy, DestroyInt);

            PrintResult("Destroy vecDestroy sets pointer to NULL", vecDestroy == NULL);
        }
    }

    printf("\n========== VECTOR TESTS END ==========\n");

    return 0;
}
