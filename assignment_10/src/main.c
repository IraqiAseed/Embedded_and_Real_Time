#include "./../include/doubleLinkedList.h"
#include <stdio.h>
#include <assert.h>

int Action(void* data, void* context)
{
    *(int*)data += *(int*)context;
    return 1;
}

int main()
{
    printf("Hello, World!\n");

    // Test 1 — Create + Empty

    List *list = ListCreate();
    assert(ListIsEmpty(list) == 1);
    assert(ListSize(list) == 0);

    printf("Test 1 - Create + Empty Success\n");

    // Test 2 Push head
    int a = 1, b = 2, c = 3;

    ListPushHead(list, &a);
    ListPushHead(list, &b);
    ListPushHead(list, &c);

    ListItr itr = ListItrBegin(list);

    // first is c = 3
    assert(*(int *)ListItrGet(itr) == 3);

    // second b = 2
    itr = ListItrNext(itr);
    assert(*(int *)ListItrGet(itr) == 2);

    // third a = 1
    itr = ListItrNext(itr);
    assert(*(int *)ListItrGet(itr) == 1);

    printf("Test 2 - Push Head Success\n");

    // list  [b] 3 2 1 [e]

    // Test 3 Push Tail
    int d = 1, e = 2, f = 3;
    ListPushTail(list, &d);
    ListPushTail(list, &e);
    ListPushTail(list, &f);

    // list should be [b] 3 2 1 1 2 3 [e]
    itr = ListItrNext(itr);
    assert(*(int *)ListItrGet(itr) == 1);

    itr = ListItrNext(itr);
    assert(*(int *)ListItrGet(itr) == 2);

    itr = ListItrNext(itr);
    assert(*(int *)ListItrGet(itr) == 3);

    // list end
    itr = ListItrNext(itr);
    assert(itr == ListItrEnd(list));
    printf("Test 3 - Push Tail Success\n");

    // Test 4 — Iteration
    printf("Test 4 - list iteration - print list\n");
    for (itr = ListItrBegin(list);
         itr != ListItrEnd(list);
         itr = ListItrNext(itr))
    {
        printf("%d ", *(int *)ListItrGet(itr));
    }

    printf("\nTest 4 - list iteration Success\n");

    // Test 5 — PopHead
    int *val = (int *)ListPopHead(list);
    assert(*val == 3);
    printf("Test 5 - Pop Head Success\n");

    // Test 6 — PopTail
    ListPopTail(list);
    val = (int *)ListPopTail(list);
    assert(*val == 2);
    printf("Test 6 - Pop Tail Success (twice)\n");

    
    // Test InsertBefore 
    // insert before second 1 in list
    // cuurent list is [b] [2 1 1] [e]
    printf("Test 7 - cuurent list is [b] [2 1 1] [e] insertBefore second 1\n");
    itr = ListItrBegin(list);
    int count = 0;
    while (itr != ListItrEnd(list))
    {
        int val = *(int *)ListItrGet(itr);

        if (val == 1)
        {
            count++;
            if (count == 2)
            {
                break;
            }
        }

        itr = ListItrNext(itr);
    }

    int g = 99;
    ListItrInsertBefore(itr,&g);

    //print list
    for (ListItr itr = ListItrBegin(list);
         itr != ListItrEnd(list);
         itr = ListItrNext(itr))
    {
        printf("%d ", *(int *)ListItrGet(itr));
    }
    printf("\n");
    printf("Test 7 - insertBefore Success\n");

   
    //Test 8 set 100 instead of 99
    int h = 100;
    ListItrSet(ListItrPrev(itr),&h);
    
    for (ListItr itr = ListItrBegin(list);
         itr != ListItrEnd(list);
         itr = ListItrNext(itr))
    {
        printf("%d ", *(int *)ListItrGet(itr));
    }
    printf("\n");
    printf("Test 8 - Set Success\n");

    
    //Test 9 — ForEach
    int someInteger = 4;
    ListItrForEach(ListItrBegin(list), ListItrEnd(list), Action, &someInteger);
    
   for (ListItr itr = ListItrBegin(list);
         itr != ListItrEnd(list);
         itr = ListItrNext(itr))
    {
        printf("%d ", *(int *)ListItrGet(itr));
    }
    printf("\n");

    printf("Test 9 - ForEach Success\n");

    ListDestroy(&list, NULL);

    return 0;
}
