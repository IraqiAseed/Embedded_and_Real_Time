#ifndef REORDER_ARRAY_H
#define REORDER_ARRAY_H

#include <stddef.h>
#include <stdio.h>

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
int array_reorder(int arr[], size_t size, size_t *evens);
/**
 * Prints the contents of an array.
 * @param arr  Pointer to the array.
 * @param size Number of elements in the array.
 * @return 0 on success.
 * @return -1 if the array pointer is NULL or size is zero.
 */
int print_array(int arr[], size_t size);

/**
 * Runs a test case for the array_reorder function.
 *
 * This function prints the array before and after calling array_reorder().
 * It also prints the number of even values found in the array.
 * @param arr  Pointer to the array to test.
 * @param size Number of elements in the array.
 * @param name Name of the test case to display in the output.
 */

void run_test(int arr[], size_t size, const char *name);

#endif