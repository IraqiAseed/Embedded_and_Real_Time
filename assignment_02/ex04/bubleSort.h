#ifndef BUBLESORT_H
#define BUBLESORT_H

#include<stddef.h>
#include<stdio.h>

/**
Sorts an array of integers using the Bubble Sort algorithm.
The function sorts the given array in ascending order by repeatedly
comparing adjacent elements and swapping them if they are in the
wrong order. The process continues until the array is fully sorted.
@param arr Pointer to the integer array to be sorted.
@param size Number of elements in the array.
@return
0 - Sorting completed successfully.
-1 - Invalid input (NULL array pointer or size equals zero).
*/
int bubleSort(int arr[], size_t size);

/**
Prints all elements of an integer array.
Each element is printed separated by a tab, followed by a newline.
@param arr Pointer to the array to print.
@param size Number of elements in the array.
@return
0 - Printed successfully.
-1 - Invalid input (NULL pointer or size equals zero).
*/
int print_array(int arr[], size_t size);

/**
Executes a test case for the Bubble Sort implementation.
The function prints the array before sorting, runs Bubble Sort,
and then prints the sorted array.
@param arr Pointer to the array to test.
@param size Number of elements in the array.
@param name Name/description of the test case.
*/
void run_test(int arr[], size_t size, const char *name);

#endif