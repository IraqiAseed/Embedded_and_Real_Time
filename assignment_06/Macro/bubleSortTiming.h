#ifndef BUBLESORTTIMING_H
#define BUBLESORT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

//same as bubleSort but with macro swap
int bubleSort_Macro(int arr[], size_t size);

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
Structure representing a sorting algorithm descriptor.

This structure encapsulates both a human-readable name of a sorting
algorithm and a pointer to the function that implements the algorithm.
It is primarily used to generalize testing, benchmarking, and comparison
between multiple sorting implementations.

@struct sortAlgorithm

@field name A constant string representing the name of the sorting algorithm.
            Used for logging, debugging, and displaying test results.

@field sortptr A function pointer to the sorting implementation.
               The function must match the following signature:

                int sort_function(int arr[], size_t size);

                Parameters:
                arr - Pointer to the array to be sorted.
                size - Number of elements in the array.

                Return:
                0 - Success
                -1 - Failure (e.g., invalid input)

            @note
            The array is sorted in-place.
**/

typedef struct sortAlgorithm
{
    const char *name;
    int (*sortptr)(int[], size_t);
} sortAlgorithm;


/**
Executes a test case for the Bubble Sort implementation.
The function prints the array before sorting, runs Bubble Sort,
and then prints the sorted array.
@param arr Pointer to the array to test.
@param size Number of elements in the array.
@param name Name/description of the test case.
@param algo A sortAlgorithm structure describing the sorting algorithm to execute.
*/
void run_test(int arr[], size_t size, const char *name, sortAlgorithm algo);





#endif