#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stdio.h>

/**
 * Performs binary search on a sorted array of integers.
 *
 * The function searches for the specified number in a sorted array
 * using the binary search algorithm.
 * The array must be sorted in ascending order. If the number is found,
 * the function stores its index in the output parameter `index`.
 *
 * @param arr    Pointer to the sorted integer array.
 * @param size   Number of elements in the array.
 * @param number Value to search for in the array.
 * @param index  Output parameter that receives the index of the found value.
 *
 * @return 0 on success (number found).
 * @return -1 if:
 *         - arr is NULL
 *         - index is NULL
 *         - size is zero
 *         - the number is not found in the array
 */
int binary_search(int arr[], size_t size, int number, int *index);

/**
 * Prints the contents of an integer array.
 *
 * Prints all elements of the array separated by tabs.
 *
 * @param arr  Pointer to the integer array.
 * @param size Number of elements in the array.
 *
 * @return 0 on success.
 * @return -1 if arr is NULL or size is zero.
 */
int print_array(int arr[], size_t size);

/**
 * Runs a test case for the binary search implementation.
 *
 * The function prints the test name, the array contents, and the result
 * of searching for the specified number using `binary_search`.
 *
 * @param arr    Pointer to the sorted integer array.
 * @param size   Number of elements in the array.
 * @param number Value to search for.
 * @param name   Name of the test case displayed in the output.
 */
void run_test(int arr[], size_t size, int number, const char *name);

#endif
