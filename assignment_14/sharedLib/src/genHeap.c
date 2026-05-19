#include "./../include/genHeap.h"
#include "genHeap.h"
#include <stdlib.h>

static void HeapifyDown(Heap *_heap, size_t _index);
static void HeapifyUp(Heap *_heap, size_t _index);
struct Heap
{
    Vector *m_vec;
    size_t m_heapSize;
    Comparator m_pfComp;
};

Heap *HeapBuild(Vector *_vector, Comparator _pfComp)
{
    if (_vector == NULL || _pfComp == NULL)
    {
        return NULL;
    }

    Heap *heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL)
    {
        return NULL;
    }

    heap->m_vec = _vector;
    heap->m_heapSize = VectorSize(_vector);
    heap->m_pfComp = _pfComp;

    // start from first leaf, then iterate backwards to last parent down to root
    size_t i = (heap->m_heapSize) / 2;

    while (i > 0)
    {
        i--;
        HeapifyDown(heap, i);
    }

    return heap;
}

Vector *HeapDestroy(Heap **_heap)
{
    if (_heap == NULL || *_heap == NULL)
    {
        return NULL;
    }

    Vector *vec = (*_heap)->m_vec;

    free(*_heap);
    *_heap = NULL;

    return vec;
}

HeapResultCode HeapInsert(Heap *_heap, void *_element)
{
    if (_heap == NULL || _heap->m_pfComp == NULL || _heap->m_vec == NULL)
    {
        return HEAP_NOT_INITIALIZED;
    }

    VectorResult res = VectorAppend(_heap->m_vec, _element);
    if (res != VECTOR_SUCCESS)
    {
        return HEAP_REALLOCATION_FAILED;
    }

    _heap->m_heapSize++;

    HeapifyUp(_heap, _heap->m_heapSize - 1);

    return HEAP_SUCCESS;
}

const void *HeapPeek(const Heap *_heap)
{
    void *top;

    if (_heap == NULL || _heap->m_pfComp == NULL || _heap->m_vec == NULL ||
        _heap->m_heapSize == 0)
    {
        return NULL;
    }

    VectorGet(_heap->m_vec, 0, &top);

    return (const void *)top;
}

void *HeapExtract(Heap *_heap)
{
    void *top;
    void *last;

    if (_heap == NULL || _heap->m_pfComp == NULL || _heap->m_vec == NULL ||
        _heap->m_heapSize == 0)
    {
        return NULL;
    }

    VectorGet(_heap->m_vec, 0, &top);

    // VectorGet(_heap->m_vec, _heap->m_heapSize - 1, &last);
    VectorRemove(_heap->m_vec, &last); // remove last element

    _heap->m_heapSize--;

    if (_heap->m_heapSize == 0)
    {
        return top;
    }

    // move last element to top and heapify down
    VectorSet(_heap->m_vec, 0, last);

    HeapifyDown(_heap, 0);

    return top;
}

size_t HeapSize(const Heap *_heap)
{
    if (_heap == NULL || _heap->m_pfComp == NULL || _heap->m_vec == NULL)
    {
        return 0;
    }

    return _heap->m_heapSize;
}

size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
    if (_heap == NULL || _heap->m_pfComp == NULL || 
        _heap->m_vec == NULL || _act == NULL)
    {
        return 0;
    }

    size_t count = 0;
    void *elem;

    for (size_t i = 0; i < _heap->m_heapSize; i++)
    {
        VectorGet(_heap->m_vec, i, &elem);
        if (_act(elem, _context) == 0)
        {
            break;
        }
        count++;
    }

    return count;
}

static void HeapifyUp(Heap *_heap, size_t _index)
{
    size_t parent_index;
    void *parent_elem;
    void *current_elem;

    if (_heap == NULL || _index >= _heap->m_heapSize || _heap->m_pfComp == NULL ||
        _heap->m_vec == NULL)
    {
        return;
    }

    while (_index > 0)
    {
        parent_index = (_index - 1) / 2;

        VectorGet(_heap->m_vec, parent_index, &parent_elem);
        VectorGet(_heap->m_vec, _index, &current_elem);

        // current is better than parent, swap
        if (_heap->m_pfComp(current_elem, parent_elem))
        {
            VectorSet(_heap->m_vec, parent_index, current_elem);
            VectorSet(_heap->m_vec, _index, parent_elem);
            _index = parent_index;
        }
        else
        {
            break;
        }
    }
}
static void HeapifyDown(Heap *_heap, size_t _index)
{
    void *parent;
    void *best_child;
    void *left;
    void *right;
    size_t index_left_child;
    size_t index_right_child;
    size_t best_child_index;

    if (_heap == NULL || _index >= _heap->m_heapSize || _heap->m_pfComp == NULL ||
        _heap->m_vec == NULL)
    {
        return;
    }

    index_left_child = 2 * _index + 1;

    while (index_left_child < _heap->m_heapSize)
    {
        index_right_child = 2 * _index + 2;

        VectorGet(_heap->m_vec, index_left_child, &left);

        best_child_index = index_left_child;

        if (index_right_child < _heap->m_heapSize) // right child exists
        {

            VectorGet(_heap->m_vec, index_right_child, &right);

            if (_heap->m_pfComp(right, left)) // right child is better than left
            {
                best_child_index = index_right_child;
            }
        }

        VectorGet(_heap->m_vec, _index, &parent);
        VectorGet(_heap->m_vec, best_child_index, &best_child);

        if (_heap->m_pfComp(best_child, parent)) // swap parent with best child
        {
            void *temp = parent;
            VectorSet(_heap->m_vec, _index, best_child);
            VectorSet(_heap->m_vec, best_child_index, temp);
            _index = best_child_index;
        }
        else
        {
            break;
        }

        index_left_child = 2 * _index + 1;
    }
}
