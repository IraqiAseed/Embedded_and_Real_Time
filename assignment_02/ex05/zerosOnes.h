#ifndef ZEROS_ONES_H
#define ZEROS_ONES_H

#include <stddef.h>
#include <stdio.h>
#include<stdbool.h>
/**
 * Sorts an array containing only 0s and 1s.
 *
 * The function rearranges the given array on-place so that all zeros appear
 * at the beginning of the array and all ones appear after them.
 * The function also validates that the array contains only the values
 * 0 and 1. If any other value is found the function fails.
 *
 * Example:
 * Input : [1,0,1,0,1]
 * Output: [0,0,1,1,1]
 *
 * @param arr  Pointer to the array.
 * @param size Number of elements in the array.
 *
 * @return 0 on success.
 * @return -1 if:
 *         - arr is NULL
 *         - size is zero
 *         - the array contains values other than 0 or 1.
 */
int sort_zeros_ones_by_count(int arr[], size_t size);
int sort_zeros_ones_by_twoPointers(int arr[], size_t size);

/**
 * Prints an array.
 *
 * @param arr  Pointer to the array.
 * @param size Number of elements in the array.
 *
 * @return 0 on success.
 * @return -1 if arr is NULL or size is zero.
 */
int print_array(int arr[], size_t size);

/**
 * Runs a test case for the sort_zeros_ones_by_count and sort_zeros_ones_by_twoPointers functions.
 *
 * Prints the array before and after sorting.
 *
 * @param arr  Pointer to the array to test.
 * @param size Number of elements in the array.
 * @param name Name of the test case.
 * @param use_two_pointers bool choose between sort_zeros_ones_by_count or sort_zeros_ones_by_twoPointers
 */
void run_test(int arr[], size_t size,bool use_two_pointers ,const char *name);

void swap(int *a, int *b);

#endif
