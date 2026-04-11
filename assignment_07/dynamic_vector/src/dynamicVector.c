#include "../include/dynamicVector.h"
#include <stdio.h>
#include <stdlib.h>

struct Vector
{
    void **m_data;
    size_t m_originalCapacity;
    size_t m_size;
    size_t m_capacity;
    size_t m_blockSize;
};

/**
 * @brief Dynamically create a new vector object of given capacity and
 * @param[in] _initialCapacity - initial capacity, number of elements that can be stored initially
 * @param[in] _blockSize - the vector will grow or shrink on demand by this size
 * @return Vector * - on success / NULL on fail
 *
 * @warning if _blockSize is 0 the vector will be of fixed size.
 * @warning if both _initialCapacity and _blockSize are zero function will return NULL.
 */

Vector *VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }

    Vector *vec = malloc(sizeof(Vector));

    if (vec == NULL)
    {

        return NULL;
    }

    // initialcapacity is 0, blocksize is not 0, we will allocate when we append the first element,
    // so tmpData is NULL for now. if initialcap is not 0 we will allocate it now.

    void **tmpData = NULL;

    // if (_initialCapacity > 0) initialCapacity can be 0, we will allocate when we append the first element, so we will not allocate here, just set tmpData to NULL, and allocate in VectorAppend when we append the first element.
    //{
    tmpData = malloc(_initialCapacity * sizeof(void *));

    if (tmpData == NULL)
    {

        free(vec);
        return NULL;
    }
    //}

    vec->m_data = tmpData;
    vec->m_blockSize = _blockSize;
    vec->m_capacity = _initialCapacity;
    vec->m_originalCapacity = _initialCapacity;
    vec->m_size = 0;

    return vec;
}

/**
 * @brief Destroy the vector and optionally destroy its elements.
 *
 * @param[in,out] _vector  Pointer to the vector pointer. On return, set to NULL.
 * @param[in] _elementDestroy  Optional callback used to destroy each stored element.
 *                             Pass NULL if elements are not heap-allocated or
 *                             should not be freed by the vector.
 *
 * @note The vector stores pointers only. It does not know whether elements
 *       point to heap, stack, or static memory. The caller is responsible
 *       for providing the correct destroy callback when needed.
 */
void VectorDestroy(Vector **_vector, void (*_elementDestroy)(void *_item))
{
    if (_vector == NULL || *_vector == NULL)
    {
        return;
    }

    if (_elementDestroy != NULL)
    {
        for (size_t i = 0; i < (*_vector)->m_size; ++i)
        {
            _elementDestroy((*_vector)->m_data[i]);
        }
    }

    free((*_vector)->m_data);
    free(*_vector);
    *_vector = NULL;
}

VectorResult VectorAppend(Vector *_vector, void *_item)
{

    if (_vector == NULL)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    if (_item == NULL)
    {
        return VECTOR_INVALID_ARGUMENT_ERROR;
    }

    if (_vector->m_capacity == _vector->m_size)
    {

        if (_vector->m_blockSize == 0)
        {
            return VECTOR_FIXED_SIZE_ERROR;
        }

        void **tmp = realloc(_vector->m_data, sizeof(void *) * (_vector->m_capacity + _vector->m_blockSize));

        if (tmp == NULL)
        {
            return VECTOR_ALLOCATION_ERROR;
        }

        _vector->m_data = tmp;
        _vector->m_capacity += _vector->m_blockSize;
    }

    _vector->m_data[_vector->m_size] = _item;
    _vector->m_size++;

    return VECTOR_SUCCESS;
}
static VectorResult VectorShrink(Vector *_vector)
{
    if (_vector->m_blockSize > 0 &&
        _vector->m_capacity > _vector->m_originalCapacity &&
        _vector->m_capacity - _vector->m_size >= 2 * _vector->m_blockSize)

    {
        void **tmp = realloc(_vector->m_data, sizeof(void *) * (_vector->m_capacity - _vector->m_blockSize));

        if (tmp == NULL)
        {
            return VECTOR_ALLOCATION_ERROR;
        }

        _vector->m_data = tmp;
        _vector->m_capacity -= _vector->m_blockSize;
    }

    return VECTOR_SUCCESS;
}
VectorResult VectorRemove(Vector *_vector, void **_pValue)
{
    if (_vector == NULL)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    if (_pValue == NULL)
    {
        return VECTOR_INVALID_ARGUMENT_ERROR;
    }
    if (_vector->m_size == 0)
    {
        return VECTOR_SIZE_IS_ZERO_ERROR;
    }

    void *ptr = _vector->m_data[_vector->m_size - 1];
    *_pValue = ptr;
    //_vector->m_data[_vector->m_size - 1] = NULL;
    _vector->m_size--;

    VectorShrink(_vector);
    // we will ignore the realloc error when shrinking, just keep the old data and capacity

    return VECTOR_SUCCESS;
}

VectorResult VectorGet(const Vector *_vector, size_t _index, void **_pValue)
{
    if (_vector == NULL)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    if(_pValue == NULL)
    {
        return VECTOR_INVALID_ARGUMENT_ERROR;
    }
    if (_index >= _vector->m_size)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }

    *_pValue = _vector->m_data[_index];

    return VECTOR_SUCCESS;
}

VectorResult VectorSet(Vector *_vector, size_t _index, void *_value)
{

    if (_vector == NULL)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    if (_index >= _vector->m_size)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }

    _vector->m_data[_index] = _value;

    return VECTOR_SUCCESS;
}

size_t VectorSize(const Vector *_vector)
{
    return _vector == NULL ? 0 : _vector->m_size;
}

size_t VectorCapacity(const Vector *_vector)
{
    return _vector == NULL ? 0 : _vector->m_capacity;
}

size_t VectorForEach(const Vector *_vector, VectorElementAction _action, void *_context)
{
    if (_vector == NULL || _action == NULL)
    {
        return 0;
    }

    void *element = NULL;
    size_t counter = 0;

    for (size_t i = 0; i < _vector->m_size; ++i)
    {

        if (VectorGet(_vector, i, &element) == VECTOR_SUCCESS)
        {
            counter++;
            if (_action(element, i, _context) == 0)
            {
                break;
            }
        }
    }

    return counter;
}
