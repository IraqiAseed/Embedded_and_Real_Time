#include "./../include/HashMap.h"
#include "./../include/doubleLinkedList.h"
#include <stdlib.h>

struct HashMap
{
    List **m_data;
    HashFunction m_hashFunction;
    EqualityFunction m_equalityFunction;
    size_t m_hashSize;   // original size given by the client
    size_t m_capacity;   // number of buckets in table
    size_t m_numOfPairs; // number of key-value pairs
};
typedef struct Pair
{
    void *m_key;
    void *m_value;
} Pair;

static size_t isPrime(size_t num);
static size_t nextPrime(size_t number);

HashMap *HashMap_Create(size_t _size, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
    HashMap *map;
    if (_hashFunc == NULL || _keysEqualFunc == NULL)
    {
        return NULL;
    }
    map = (HashMap *)malloc(sizeof(HashMap));

    if (NULL == map)
    {
        return NULL;
    }
    map->m_hashFunction = _hashFunc;
    map->m_equalityFunction = _keysEqualFunc;
    map->m_hashSize = _size;

    map->m_capacity = nextPrime(_size);
    map->m_numOfPairs = 0;

    // map->m_data = calloc(map->m_capacity, sizeof(List *));
    map->m_data = calloc(map->m_capacity, sizeof(*(map->m_data)));
    if (map->m_data == NULL)
    {
        free(map);
        return NULL;
    }

    return map;
}

void HashMap_Destroy(HashMap **_map, void (*_keyDestroy)(void *_key), void (*_valDestroy)(void *_value))
{
    if (_map == NULL || *_map == NULL)
    {
        return;
    }

    if ((*_map)->m_data == NULL)
    {
        free(*_map);
        *_map = NULL;
        return;
    }

    for (size_t i = 0; i < (*_map)->m_capacity; ++i)
    {
        if ((*_map)->m_data[i] == NULL)
        {
            continue;
        }

        ListItr itr = ListItrBegin((*_map)->m_data[i]);
        ListItr end = ListItrEnd((*_map)->m_data[i]);

        while (itr != end)
        {
            ListItr next = ListItrNext(itr);
            Pair *pair = (Pair *)ListItrGet(itr);

            if (pair != NULL)
            {
                if (_keyDestroy != NULL)
                {
                    _keyDestroy(pair->m_key);
                }

                if (_valDestroy != NULL)
                {
                    _valDestroy(pair->m_value);
                }

                free(pair);
            }

            ListItrRemove(itr);
            itr = next;
        }

        ListDestroy(&((*_map)->m_data[i]), NULL);
    }

    free((*_map)->m_data);
    free(*_map);
    *_map = NULL;
}

Map_Result HashMap_Insert(HashMap *_map, const void *_key, const void *_value)
{
    if (_map == NULL || _map->m_data == NULL ||
        _map->m_capacity == 0 || _map->m_hashFunction == NULL ||
        _map->m_equalityFunction == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }

    size_t index = _map->m_hashFunction((void *)_key) % _map->m_capacity;

    Pair *pair = (Pair *)malloc(sizeof(Pair));

    if (pair == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }
    pair->m_key = (void *)_key;
    pair->m_value = (void *)_value;

    if (_map->m_data[index] == NULL)
    {
        _map->m_data[index] = ListCreate();
        if (_map->m_data[index] == NULL)
        {
            free(pair);
            return MAP_ALLOCATION_ERROR;
        }
    }
    else
    {
        // TODO  - optimize search for duplicate key by using hash function and equality function
        //  instead of iterating though the bucket list
        //  then redesign the code avoid frees in case of duplicate key error
        // use push head instead push tail !!!
        void *existingValue = NULL;
        Map_Result findResult = HashMap_Find(_map, _key, &existingValue);
        if (findResult == MAP_SUCCESS)
        {
            free(pair);
            return MAP_KEY_DUPLICATE_ERROR;
        }
    }

    ListItr pushResult = ListPushTail(_map->m_data[index], (void *)pair);
    if (pushResult == NULL)
    {
        free(pair);
        return MAP_ALLOCATION_ERROR;
    }
    _map->m_numOfPairs++;

    return MAP_SUCCESS;
}

Map_Result HashMap_Remove(HashMap *_map, const void *_searchKey, void **_pKey, void **_pValue)
{
    if (_map == NULL || _map->m_data == NULL ||
        _map->m_capacity == 0 || _map->m_hashFunction == NULL ||
        _map->m_equalityFunction == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_searchKey == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if (_pKey == NULL || _pValue == NULL)
    {
        return MAP_OUTPUT_PARAMETER_NULL_ERROR;
    }

    size_t index = _map->m_hashFunction((void *)_searchKey) % _map->m_capacity;
    List *bucket = _map->m_data[index];
    if (bucket == NULL)
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }

    ListItr itr = ListItrBegin(bucket);
    ListItr end = ListItrEnd(bucket);

    while (itr != end)
    {
        Pair *pair = (Pair *)ListItrGet(itr);

        if (pair != NULL && _map->m_equalityFunction((void *)_searchKey, pair->m_key))
        {
            *_pKey = pair->m_key;
            *_pValue = pair->m_value;

            ListItrRemove(itr);
            free(pair);
            _map->m_numOfPairs--;

            if (ListIsEmpty(bucket))
            {
                ListDestroy(&(_map->m_data[index]), NULL);
            }

            return MAP_SUCCESS;
        }
        itr = ListItrNext(itr);
    }
    return MAP_KEY_NOT_FOUND_ERROR;
}

Map_Result HashMap_Find(const HashMap *_map, const void *_key, void **_pValue)
{
    if (_map == NULL || _map->m_data == NULL ||
        _map->m_capacity == 0 || _map->m_hashFunction == NULL ||
        _map->m_equalityFunction == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if (_pValue == NULL)
    {
        return MAP_OUTPUT_PARAMETER_NULL_ERROR;
    }
    size_t index = _map->m_hashFunction((void *)_key) % _map->m_capacity;
    List *bucket = _map->m_data[index];

    if (bucket == NULL)
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }

    ListItr itr = ListItrBegin(bucket);
    ListItr end = ListItrEnd(bucket);

    while (itr != end)
    {
        Pair *pair = (Pair *)ListItrGet(itr);

        if (pair != NULL && _map->m_equalityFunction((void *)_key, pair->m_key))
        {
            *_pValue = pair->m_value;
            return MAP_SUCCESS;
        }
        itr = ListItrNext(itr);
    }

    return MAP_KEY_NOT_FOUND_ERROR;
}
size_t HashMap_Size(const HashMap *_map)
{
    if (_map == NULL)
    {
        return 0;
    }
    return _map->m_numOfPairs;
}

static size_t isPrime(size_t num)
{
    size_t i;
    if (num <= 1)
    {
        return 0;
    }
    for (i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}
static size_t nextPrime(size_t number)
{
    size_t num = number + 1;

    if (num <= 2)
    {
        return 2;
    }

    if (num % 2 == 0)
    {
        num++;
    }

    while (!isPrime(num))
    {
        num += 2;
    }

    return num;
}
