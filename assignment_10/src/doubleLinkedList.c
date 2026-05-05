#include "../include/doubleLinkedList.h"
#include <stdlib.h>
typedef struct Node Node;

struct Node
{
    void *m_data;
    Node *m_next;
    Node *m_prev;
};

struct List
{
    Node m_head;
    Node m_tail;
};

List *ListCreate(void)
{
    List *list = (List *)malloc(sizeof(List));

    if (list == NULL)
    {
        return NULL;
    }

    list->m_head.m_data = NULL;
    list->m_head.m_next = &list->m_tail;
    list->m_head.m_prev = &list->m_head;

    list->m_tail.m_data = NULL;
    list->m_tail.m_prev = &list->m_head;
    list->m_tail.m_next = &list->m_tail;

    return list;
}

void ListDestroy(List **_pList, void (*_elementDestroy)(void *_item))
{
    if (_pList == NULL || *_pList == NULL)
    {
        return;
    }

    Node *current = (*_pList)->m_head.m_next;

    while (current != &(*_pList)->m_tail) // cuurent is not null by design
    {
        Node *next = current->m_next;

        if (_elementDestroy != NULL)
        {
            _elementDestroy(current->m_data);
        }
        free(current);

        current = next;
    }

    free(*_pList);
    *_pList = NULL;
}
// position must not be NULL and must not be the head
// item must not be NULL
static Node *InsertBefore(Node *position, void *item)
{
    if (position == NULL || item == NULL)
    {
        return NULL;
    }
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
    {
        return NULL;
    }
    node->m_data = item;

    node->m_prev = position->m_prev;
    node->m_next = position;
    position->m_prev->m_next = node;
    position->m_prev = node;

    return node;
}

ListItr ListPushHead(List *_list, void *_item)
{
    if (_list == NULL || _item == NULL)
    {
        return NULL;
    }

    // first can be the tail node if the list is empty
    Node *first = _list->m_head.m_next;

    return (ListItr)InsertBefore(first, _item);
}

ListItr ListPushTail(List *_list, void *_item)
{
    if (_list == NULL || _item == NULL)
    {
        return NULL;
    }

    return (ListItr)InsertBefore(&_list->m_tail, _item);
}

static void *RemoveNode(Node *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    // sentinel cant be removed by design
    if (node->m_prev == node || node->m_next == node)
    {
        return NULL;
    }

    node->m_prev->m_next = node->m_next;
    node->m_next->m_prev = node->m_prev;

    void *data = node->m_data;
    free(node);
    return data;
}

void *ListPopHead(List *_list)
{
    // check if list is empty or null, and head can not be poped by design
    if (_list == NULL || (_list->m_head.m_next == &_list->m_tail))
    {
        return NULL;
    }

    return RemoveNode(_list->m_head.m_next);
}

void *ListPopTail(List *_list)
{
    // check if list is empty or null, and tail can not be poped by design
    if (_list == NULL || (_list->m_tail.m_prev == &_list->m_head))
    {
        return NULL;
    }

    return RemoveNode(_list->m_tail.m_prev);
}

ListItr ListItrBegin(const List *_list)
{
    if (_list == NULL)
    {
        return NULL;
    }

    return (ListItr)_list->m_head.m_next;
}

ListItr ListItrEnd(const List *_list)
{
    if (_list == NULL)
    {
        return NULL;
    }

    return (ListItr)&_list->m_tail;
}

ListItr ListItrNext(ListItr _itr)
{

    // check if itr is null or tail by design, because tail next is tail itself
    if (_itr == NULL || ((Node *)_itr)->m_next == (Node *)_itr)
    {
        return _itr;
    }

    return (ListItr)(((Node *)_itr)->m_next);
}

ListItr ListItrPrev(ListItr _itr)
{
    Node *node = (Node *)_itr;

    // check if node is null or node is head by design, because head prev is head itself
    if (node == NULL || node->m_prev == node)
    {
        return _itr;
    }
    // if node is the first real node, its prev is head.
    // Do not expose head to the user, so return the current iterator.
    if (node->m_prev->m_prev == node->m_prev)
    {
        return _itr;
    }

    return (ListItr)node->m_prev;
}

void *ListItrGet(ListItr _itr)
{

    if (_itr == NULL)
    {
        return NULL;
    }

    // sentinels (head + tail)
    if (((Node *)_itr)->m_prev == (Node *)_itr || ((Node *)_itr)->m_next == (Node *)_itr)
    {
        return NULL;
    }

    return ((Node *)_itr)->m_data;
}

void *ListItrSet(ListItr _itr, void *_element)
{
    Node *node = (Node *)_itr;

    if (node == NULL || _element == NULL)
    {
        return NULL;
    }

    // prevent  sentinels set
    if (node->m_prev == node || node->m_next == node)
    {
        return NULL;
    }

    void *oldData = node->m_data;
    node->m_data = _element;

    return oldData;
}

ListItr ListItrInsertBefore(ListItr _itr, void *_element)
{
    Node *node = (Node *)_itr;

    if (node == NULL || _element == NULL || node->m_prev == node)
    {
        return NULL;
    }

    return (ListItr)InsertBefore(node, _element);
}

    void *ListItrRemove(ListItr _itr)
    {
        Node *node = (Node *)_itr;

        if (node == NULL ||
            node->m_next == node || // tail
            node->m_prev == node)   // head
        {
            return NULL;
        }

        return RemoveNode(node);
    }

    size_t ListSize(const List *_list)
    {
        size_t size = 0;

        if (_list == NULL)
        {
            return size;
        }

        for (ListItr itr = ListItrBegin(_list);
             itr != ListItrEnd(_list);
             itr = ListItrNext(itr))
        {
            ++size;
        }

        return size;
    }

    size_t ListIsEmpty(List * _list)
    {
        if (_list == NULL)
        {
            return 1;
        }

        return (_list->m_head.m_next == &_list->m_tail) ? 1 : 0;
    }

    ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void *_context)
    {
        if (_begin == NULL || _end == NULL || _action == NULL)
        {
            return NULL;
        }

        ListItr current = _begin;

        while (current != _end)
        {
            void *data = ListItrGet(current);

            if (data == NULL)
            {
                break; // range validation
            }

            if (_action(data, _context) == 0)
            {
                break;
            }

            current = ListItrNext(current);
        }

        return current;
    }
