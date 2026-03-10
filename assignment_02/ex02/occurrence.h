#ifndef OCCURRENCE_H
#define OCCURRENCE_H

#include<stdio.h>


/**
 * Find the value with the maximum occurrence in an integer array, not using additional memory
 * not using additional array !!
 *
 * This function scans the given array and determines which value appears
 * the most times. It returns that value through max_occurrence_value and
 * the number of times it appears through occurrences.
 *
 * The function does not modify the input array.
 *
 * @param arr                     Pointer to the first element of the array.
 * @param size                    Number of elements in the array.
 * @param max_occurrence_value    Output parameter that will store the value
 *                                that appears most frequently.
 * @param occurrences             Output parameter that will store the number
 *                                of times the value appears.
 *
 * @return 0 on success.
 * @return -1 if invalid parameters are provided (NULL pointer or size == 0).
 */

int max_occurrence(const int arr[],size_t size, int* max_occurrence_value,int* occurrences);


#endif