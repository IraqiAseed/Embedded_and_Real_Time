#include "./../include/singleLinkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleLinkedlist.h"

struct Person
{
    int m_id; // primary key
    char m_name[128];
    int m_age;
    Person *m_next;
};

Person *ListInsertHead(Person *_head, Person *_p)
{

    if (_p == NULL)
    {
        return _head;
    }

    _p->m_next = _head;

    return _p;
}
Person *ListRemoveHead(Person *_head, Person **_item)
{
    if (_head == NULL || _item == NULL)
    {
        return _head;
    }

    *_item = _head;
    _head = _head->m_next;

    //(*_item)-> m_next = NULL;

    return _head;
}

Person *ListInsertByKey(Person *_head, int _key, Person *_p)
{
    Person *curr;
    Person *prev;

    if (_p == NULL || _key != _p->m_id)
    {
        return _head;
    }

    if (_head == NULL)
    {
        return _p;
    }

    if (_p->m_id < _head->m_id)
    {
        _p->m_next = _head;
        return _p;
        // return ListInsertHead(_head,_p);
    }

    prev = NULL;
    curr = _head;

    while (curr)
    {
        if (_p->m_id > curr->m_id)
        {
            prev = curr;
            curr = curr->m_next;
        }
        else
        {
            break;
        }
    }

    // check duplicates
    if (curr != NULL && _p->m_id == curr->m_id)
    {
        return _head;
    }

    prev->m_next = _p;
    _p->m_next = curr;

    return _head;
}

Person *ListRemoveByKey(Person *_head, int _key, Person **_p)
{
    Person *curr = _head;
    Person *prev = NULL;

    if (_head == NULL || _p == NULL)
    {
        return _head;
    }

    *_p = NULL;

    while (curr)
    {
        if (curr->m_id == _key)
        {
            if (prev == NULL)
            {
                *_p = _head;
                _head = _head->m_next;
            }
            else
            {
                *_p = curr;
                prev->m_next = curr->m_next;
            }

            (*_p)->m_next = NULL;
            break;
        }

        prev = curr;
        curr = curr->m_next;
    }

    return _head;
}

 void PersonPrint(const Person *p)
{
    if (p == NULL)
        return;

    printf("ID: %d, Name: %s, Age: %d\n",
           p->m_id, p->m_name, p->m_age);
}
void PrintList(Person *_head)
{
    while (_head)
    {
        PersonPrint(_head);
        _head = _head->m_next;
    }
}

Person *PersonCreate(int _id, const char *_name, int _age)
{
    Person *p;

    if (_name == NULL)
    {
        return NULL;
    }

    p = (Person *)malloc(sizeof(Person));

    if (p == NULL)
    {
        return NULL;
    }

    p->m_id = _id;
    strncpy(p->m_name, _name, sizeof(p->m_name) - 1);
    p->m_name[sizeof(p->m_name) - 1] = '\0';
    p->m_age = _age;
    p->m_next = NULL;

    return p;
}

void PersonDestroy(Person *_p)
{
    free(_p);
}
Person* ListGetLastNode(Person* _head)
{
    if(_head == NULL)
    {
        return NULL;
    }

    if(_head->m_next == NULL)
    {
        return _head;
    }

    ListGetLastNode(_head->m_next);

    return ListGetLastNode(_head->m_next);
}

Person* ListReverse(Person* _head)
{
    if(_head == NULL)
    {
        return NULL;
    }

    if(_head->m_next == NULL)
    {
        return _head;
    }

    _head->m_next = ListReverse(_head->m_next);

    return _head;
}


Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p)
{
    if (_p == NULL || _key != _p->m_id)
    {
        return _head;
    }

    if (_head == NULL)
    {
        _p->m_next = NULL;
        return _p;
    }

    if (_p->m_id < _head->m_id)
    {
        _p->m_next = _head;
        return _p;
    }

    if (_p->m_id == _head->m_id)
    {
        return _head;
    }

    _head->m_next = ListInsertByKeyRec(_head->m_next, _key, _p);
    return _head;
}

Person* ListRemoveByKeyRec(Person* _head, int _key, Person** _p)
{
    if (_p == NULL)
    {
        return _head;
    }

    if (_head == NULL)
    {
        *_p = NULL;
        return NULL;
    }

    if (_head->m_id == _key)
    {
        Person* next = _head->m_next;
        *_p = _head;
        _head->m_next = NULL;
        return next;
    }

    _head->m_next = ListRemoveByKeyRec(_head->m_next, _key, _p);
    return _head;
}
