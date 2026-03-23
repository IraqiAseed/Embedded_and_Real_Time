#ifndef WC_H_
#define WC_H_

#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>


/**
 * Counts the number of words, characters, and lines in a file.
 *
 * This function reads the file specified by `path` and computes:
 *   - Total number of words
 *   - Total number of characters
 *   - Total number of lines
 *
 * The results are returned via output parameters.
 *
 * @param path Path to the input file to analyze.
 *
 * @param word_count Output parameter. Will contain the number of words found in the file.
 *                   Must not be NULL.
 *
 * @param char_count Output parameter. Will contain the number of characters in the file.
 *                   Must not be NULL.
 *
 * @param line_count Output parameter. Will contain the number of lines in the file.
 *                   Must not be NULL.
 *
 * @return
 *        0  - Success.
 *       -1  - Failure (e.g., file open error, invalid parameters).
 *
 * @note
 *        - A "word" is typically defined as a sequence of non-whitespace characters.
 *        - Lines are usually counted by the number of newline characters ('\n').
 *        - Character count includes all characters read from the file, including whitespace.
 *
 *
 */
int wc_function(const char* path, int* word_count, int* char_count, int* line_count);

#endif // WC_H_