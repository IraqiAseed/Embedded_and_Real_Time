#include <stdio.h>
#include "../include/circularQueue.h"

int main(void)
{
    Queue* q = QueueCreate(5);

    if (q == NULL)
    {
        printf("QueueCreate failed\n");
        return 1;
    }

    int x = 10;
    int y = 20;
    int* removed = NULL;

    QueueInsert(q, &x);
    QueueInsert(q, &y);

    QueueRemove(q, (void**)&removed);

    printf("Removed value = %d\n", *removed);

    QueueDestroy(&q, NULL);

    return 0;
}