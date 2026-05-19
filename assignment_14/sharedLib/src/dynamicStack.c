#include "../include/dynamicStack.h"
#include "../include/dynamicVector.h"
#include <stdlib.h>
#include <stdio.h>

struct Stack
{
    Vector *m_vector;
};

static StackResult MapVectorResultToStackResult(VectorResult _result)
{
    switch (_result)
    {
    case VECTOR_SUCCESS:
        return STACK_SUCCESS;

    case VECTOR_UNITIALIZED_ERROR:
        return STACK_UNITIALIZED_ERROR;

    case VECTOR_ALLOCATION_ERROR:
        return STACK_ALLOCATION_ERROR;

    case VECTOR_FIXED_SIZE_ERROR:
        return STACK_FIXED_SIZE_ERROR;

    case VECTOR_SIZE_IS_ZERO_ERROR:
        return STACK_IS_EMPTY;

    case VECTOR_INVALID_ARGUMENT_ERROR:
        return STACK_INVALID_ARGUMENT_ERROR;

    default:
        return STACK_GENERAL_ERROR;
    }
}
Stack *StackCreate(size_t _initialCapacity, size_t _blockSize)
{

    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return NULL;
    }

    Vector *vector = VectorCreate(_initialCapacity, _blockSize);

    if (vector == NULL)
    {
        free(stack);
        return NULL;
    }

    stack->m_vector = vector;

    return stack;
}
void StackDestroy(Stack **_stack, void (*_elementDestroy)(void *_item))
{
    if (_stack == NULL || *_stack == NULL)
    {
        return;
    }

    VectorDestroy(&((*_stack)->m_vector), _elementDestroy);

    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack *_stack, void *_item)
{
    if (_stack == NULL)
    {
        return STACK_UNITIALIZED_ERROR;
    }

    VectorResult result = VectorAppend(_stack->m_vector, _item);

    return MapVectorResultToStackResult(result);
}

StackResult StackPop(Stack *_stack, void **_pValue)
{
    if (_stack == NULL)
    {
        return STACK_UNITIALIZED_ERROR;
    }

    VectorResult result = VectorRemove(_stack->m_vector, _pValue);

    return MapVectorResultToStackResult(result);
}

StackResult StackTop(Stack *_stack, void **_pValue)
{

    if (_stack == NULL)
    {
        return STACK_UNITIALIZED_ERROR;
    }

    // need to check if stack is empty before calling VectorGet,
    // because VectorGet will return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR
    // index is size - 1, but if size is 0, index will be -1, and in size_t it will be SIZE_MAX
    // if we try to get from an empty vector,
    // but we want to return STACK_IS_EMPTY in this case
    if (StackIsEmpty(_stack))
    {
        return STACK_IS_EMPTY;
    }

    VectorResult result = VectorGet(_stack->m_vector, VectorSize(_stack->m_vector) - 1, _pValue);

    return MapVectorResultToStackResult(result);
}

size_t StackSize(const Stack *_stack)
{
    return (_stack == NULL) ? 0 : VectorSize(_stack->m_vector);
}

size_t StackCapacity(const Stack *_stack)
{
    return (_stack == NULL) ? 0 : VectorCapacity(_stack->m_vector);
}

int StackIsEmpty(Stack *_stack)
{
    if (_stack == NULL)
    {
        return 1; // treat uninitialized stack as empty
    }

    if (VectorSize(_stack->m_vector) == 0)
    {
        return 1;
    }

    return 0;
}

size_t StackForEach(const Stack *_stack, StackElementAction _action, void *_context)
{
    if( _stack == NULL)
    {
        return 0;
    }
    return VectorForEach(_stack->m_vector, (VectorElementAction)_action, _context);

}

int PrintInt(void *_element, size_t _index, void *_context)
{
    (void)_context;
    int *value = (int *)_element;
    printf("[%zu] = %d\n", _index, *value);

    return 1;
}


size_t StackPrintInt(const Stack *_stack)
{
    if (_stack == NULL)
    {
        return 0;
    }

    return StackForEach(_stack, PrintInt, NULL);
}
