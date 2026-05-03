#include "./../include/genHeap.h"
#include<stdio.h>

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

int Print(const void *element, void *context)
{
    (void)context;
    if (element == NULL)
    {
        printf("NULL\n");
    }
    else
    {
        printf("%d\n", *(int *)element);
        
    }

    return 1;
}

int IntLess(const void* a, const void* b)
{
    return *(int*)a < *(int*)b;
}

int main()
{
    Vector* vec = VectorCreate(10, 5);

    int arr[] = {20, 90, 10, 50, 70};
    for(size_t i=0; i < sizeof(arr)/sizeof(arr[0]); i++)
    {
        VectorAppend(vec, &arr[i]);
    }
    

    printf("Before HeapBuild:\n");
    VectorForEach(vec, PrintInt, NULL);

    Heap* heap = HeapBuild(vec, IntLess);

    printf("After HeapBuild:\n");
    VectorForEach(vec, PrintInt, NULL);

    printf("Heap insert: 15\n");
    int element1 = 15;
    HeapInsert(heap, &element1);
    VectorForEach(vec, PrintInt, NULL);

    printf("Heap insert: 1\n");
    int element2 = 1;
    HeapInsert(heap, &element2);
    VectorForEach(vec, PrintInt, NULL);


    printf("Heap insert: 0\n");
    int element3 = 0;
    HeapInsert(heap, &element3 );
    VectorForEach(vec, PrintInt, NULL);

    printf("Heap peek: %d\n", *(int *)HeapPeek(heap));

    void* extracted = HeapExtract(heap);
    printf("After HeapExtract:\n");
    VectorForEach(vec, PrintInt, NULL);
    
    //check if extracted element pointer is the same as element3 
    printf("Extracted element: %p\n", (void*)extracted);
    printf("Extracted element: %p\n", &element3);

    HeapForEach(heap, Print, NULL);

    Vector* v = HeapDestroy(&heap);
    VectorDestroy(&v,NULL); //it is same vector, but free it as the returned value

    return 0;
}