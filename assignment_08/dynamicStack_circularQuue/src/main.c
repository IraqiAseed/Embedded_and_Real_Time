#include "./../include/dynamicStack.h"
#include "./../include/circularQueue.h"
#include <stdio.h>

static int PrintQueueItem(void* _item, void* _context)
{
    (void)_context;

    if (_item == NULL)
    {
        return 0;
    }

    printf("%d ", *(int*)_item);
    return 1;
}

int main(void)
{
    /* =========================
       Stack tests
       ========================= */
    
    printf("\n===== Stack Tests Start =====\n");
    Stack *stack = StackCreate(2, 2);
    if (stack == NULL)
    {
        printf("Failed to create stack\n");
        return -1;
    }

    int x = 10;
    int y = 20;
    int z = 30;

    if (StackPush(stack, &x) != STACK_SUCCESS)
    {
        printf("Push x failed\n");
    }

    if (StackPush(stack, &y) != STACK_SUCCESS)
    {
        printf("Push y failed\n");
    }

    if (StackPush(stack, &z) != STACK_SUCCESS)
    {
        printf("Push z failed\n");
    }

    printf("Stack size: %zu\n", StackSize(stack));
    printf("Stack capacity: %zu\n", StackCapacity(stack));

    void *out = NULL;

    if (StackTop(stack, &out) == STACK_SUCCESS)
    {
        printf("Top = %d\n", *(int *)out);
    }
    else
    {
        printf("Top failed\n");
    }

    if (StackPop(stack, &out) == STACK_SUCCESS)
    {
        printf("Popped = %d\n", *(int *)out);
    }
    else
    {
        printf("Pop failed\n");
    }

    printf("Stack size after pop: %zu\n", StackSize(stack));

    if (StackTop(stack, &out) == STACK_SUCCESS)
    {
        printf("Top = %d\n", *(int *)out);
    }
    else
    {
        printf("Top failed\n");
    }

    StackDestroy(&stack, NULL);

    printf("\n===== Stack Tests End =====\n");

    /* =========================
       Circular Queue tests
       ========================= */
    Queue* queue = QueueCreate(3);
    if (queue == NULL)
    {
        printf("\nFailed to create queue\n");
        return -1;
    }

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    printf("\n===== Queue Tests Start =====\n");

    printf("Queue empty? %zu\n", QueueIsEmpty(queue));

    if (QueueInsert(queue, &a) == QUEUE_SUCCESS)
    {
        printf("Inserted a = %d\n", a);
    }
    else
    {
        printf("Insert a failed\n");
    }

    if (QueueInsert(queue, &b) == QUEUE_SUCCESS)
    {
        printf("Inserted b = %d\n", b);
    }
    else
    {
        printf("Insert b failed\n");
    }

    if (QueueInsert(queue, &c) == QUEUE_SUCCESS)
    {
        printf("Inserted c = %d\n", c);
    }
    else
    {
        printf("Insert c failed\n");
    }

    printf("Queue empty after inserts? %zu\n", QueueIsEmpty(queue));

    if (QueueInsert(queue, &d) == QUEUE_OVERFLOW_ERROR)
    {
        printf("Insert d failed as expected: queue overflow\n");
    }
    else
    {
        printf("Unexpected result when inserting d\n");
    }

    printf("Queue items: ");
    QueueForEach(queue, PrintQueueItem, NULL);
    printf("\n");

    if (QueueRemove(queue, &out) == QUEUE_SUCCESS)
    {
        printf("Removed = %d\n", *(int*)out);
    }
    else
    {
        printf("Remove failed\n");
    }

    if (QueueRemove(queue, &out) == QUEUE_SUCCESS)
    {
        printf("Removed = %d\n", *(int*)out);
    }
    else
    {
        printf("Remove failed\n");
    }

    printf("Queue items after 2 removes: ");
    QueueForEach(queue, PrintQueueItem, NULL);
    printf("\n");

    if (QueueInsert(queue, &d) == QUEUE_SUCCESS)
    {
        printf("Inserted d = %d\n", d);
    }
    else
    {
        printf("Insert d failed\n");
    }

    printf("Queue items after wrap-around insert: ");
    QueueForEach(queue, PrintQueueItem, NULL);
    printf("\n");

    while (!QueueIsEmpty(queue))
    {
        if (QueueRemove(queue, &out) == QUEUE_SUCCESS)
        {
            printf("Removed = %d\n", *(int*)out);
        }
    }

    printf("Queue empty at end? %zu\n", QueueIsEmpty(queue));

    if (QueueRemove(queue, &out) == QUEUE_DATA_NOT_FOUND_ERROR)
    {
        printf("Remove from empty queue failed as expected\n");
    }
    else
    {
        printf("Unexpected result when removing from empty queue\n");
    }

    QueueDestroy(&queue, NULL);

    printf("===== Queue Tests End =====\n");

    return 0;
}