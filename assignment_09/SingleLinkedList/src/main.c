#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../include/singleLinkedlist.h"



int main(void)
{
    Person* head = NULL;
    Person* removed = NULL;

    head = ListInsertByKey(head, 20, PersonCreate(20, "Alice", 30));
    head = ListInsertByKey(head, 10, PersonCreate(10, "Bob", 25));
    head = ListInsertByKey(head, 30, PersonCreate(30, "Charlie", 28));
    head = ListInsertByKey(head, 25, PersonCreate(25, "David", 22));

    printf("After ListInsertByKey:\n");
    PrintList(head);

    printf("\nInsert head:\n");
    head = ListInsertHead(head, PersonCreate(5, "Eve", 19));
    PrintList(head);

    printf("\nRemove person by key = 25\n");
    head = ListRemoveByKey(head, 25, &removed);
    if (removed != NULL)
    {
        PersonDestroy(removed);
        removed = NULL;
    }
    PrintList(head);

    printf("\nRemove head:\n");
    head = ListRemoveHead(head, &removed);
    if (removed != NULL)
    {
        PersonDestroy(removed);
        removed = NULL;
    }
    PrintList(head);

    while (head != NULL)
    {
        head = ListRemoveHead(head, &removed);
        if (removed != NULL)
        {
            PersonDestroy(removed);
            removed = NULL;
        }
    }

    return 0;
}