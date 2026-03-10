#ifndef REORDER_ARRAY_H
#define REORDER_ARRAY_H

#include<stddef.h>

/**
 * Reorders the array on-place so that all even numbers appear before odd numbers.
 * The relative order of elements is preserved (stable reorder).
 *
 * @param arr   Pointer to the array to reorder.
 * @param size  Number of elements in the array.
 * @param evens Output parameter that will contain the number of even elements.
 *
 * @return
 * 0  on success.  
 * -1 if arr or evens is NULL.
 * 
 */
int array_reorder(int arr[],size_t size,size_t* evens);

#endif