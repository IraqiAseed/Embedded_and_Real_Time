#include "./../include/circularQueue.h"
#include <stdlib.h>

struct Queue
{
    void **m_que;
    size_t m_capacity;
    size_t m_size;
    size_t m_head; /* Index of head in m_que. */
    size_t m_tail; /* Index of tail in m_que. */
};

Queue *QueueCreate(size_t _capacity)
{
    if (_capacity == 0)
    {
        return NULL;
    }

    Queue *queue = malloc(sizeof(Queue));

    if (queue == NULL)
    {
        return NULL;
    }

    queue->m_que = malloc(sizeof(void *) * _capacity);

    if (queue->m_que == NULL)
    {
        free(queue);
        return NULL;
    }

    queue->m_capacity = _capacity;
    queue->m_size = 0;
    queue->m_head = 0;
    queue->m_tail = 0;

    return queue;
}

void QueueDestroy(Queue **_queue, DestroyItem _itemDestroy)
{

    if (_queue == NULL || *_queue == NULL)
    {
        return;
    }

    if (_itemDestroy != NULL)
    {

        for (size_t i = (*_queue)->m_head, times = 0;
             times < (*_queue)->m_size;
             times++, i = (i + 1) % (*_queue)->m_capacity)
        {
            _itemDestroy((*_queue)->m_que[i]);
            //(*_queue)->m_que[i] = NULL;
        }
    }

    if ((*_queue)->m_que)
    {
        free((*_queue)->m_que);
        (*_queue)->m_que = NULL;
    }

    free(*_queue);
    *_queue = NULL;
}

QueueResult QueueInsert(Queue *_queue, void *_item)
{
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }

    if (_queue->m_size == _queue->m_capacity)
    {
        return QUEUE_OVERFLOW_ERROR;
    }

    _queue->m_que[_queue->m_tail] = _item;

    _queue->m_tail = (_queue->m_tail + 1) % _queue->m_capacity;

    _queue->m_size++;

    return QUEUE_SUCCESS;
}

QueueResult QueueRemove(Queue *_queue, void **_item)
{
    if (_queue == NULL)
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    if (QueueIsEmpty(_queue))
    {
        return QUEUE_DATA_NOT_FOUND_ERROR;
    }

    *_item = _queue->m_que[_queue->m_head];

    _queue->m_head = (_queue->m_head + 1) % _queue->m_capacity;

    _queue->m_size--;

    return QUEUE_SUCCESS;
}

size_t QueueIsEmpty(Queue *_queue)
{
    if (_queue == NULL)
    {
        return 1;
    }

    return _queue->m_size == 0;
}

size_t QueueForEach(Queue *_queue, ActionFunction _action, void *_context)
{

    size_t times = 0;
    
    if (_queue == NULL || _action == NULL)
    {
        return 0;
    }

    for (size_t i = _queue->m_head;
         times < _queue->m_size;
         times++, i = (i + 1) % _queue->m_capacity)
    {
        if (_action(_queue->m_que[i], _context) == 0)
        {
            return times + 1;
        }
    }

    return times;
}
