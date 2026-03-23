#ifndef TAIL_H_
#define TAIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Reads the last N lines from a file using a circular buffer.
 *
 * This function opens the file specified by `path` and reads up to `numOfLines`
 * lines from it, keeping only the most recent lines using a circular buffer
 * mechanism.
 *
 * The resulting lines are stored in the array pointed to by `lines`.
 * The function also returns metadata about how many lines were actually read
 * and the starting index in the circular buffer.
 *
 * @param path Path to the input file to read from.
 *
 * @param numOfLines Maximum number of lines to retain (size of the circular buffer).
 *
 * @param lines Output array of strings (char*) where the resulting lines will be stored.
 *              The caller is responsible for allocating memory for the array and freeing it.
 *
 * @param linesAdded Output parameter. Will contain the number of lines actually stored.
 *                   This may be less than `numOfLines` if the file contains fewer lines.
 *
 * @param startIndex Output parameter. Indicates the index in the circular buffer where
 *                   the oldest line starts. Useful for iterating in correct order.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (e.g., file open error, invalid parameters).
 *
 * @note
 *        - Uses circular buffer logic: when buffer is full, older lines are overwritten.
 *        - Lines are not guaranteed to be stored in chronological order unless
 *          accessed using `startIndex`.
 *
 * @warning
 *        - The function assumes valid pointers for `lines`, `linesAdded`, and `startIndex`.
 *        - The caller must ensure proper memory management for allocated strings.
 *
 */


int Circular_Buffer_Tail(const char* path,int numOfLines,char** lines,int* linesAdded,int* startIndex);
#endif // TAIL_H_
