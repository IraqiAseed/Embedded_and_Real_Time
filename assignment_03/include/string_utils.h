#ifndef STRING_UTILS_H
#define STRING_UTILS_H

/**
 * Reverse a null-terminated string in place.
 *
 * This function reverses the characters of the given string by swapping
 * characters from the beginning and the end until the middle of the
 * string is reached. The reversal is done in-place, meaning the original
 * string buffer is modified directly and no additional memory is used.
 *
 * @param str Pointer to a null-terminated string to be reversed.
 *            The string must reside in writable memory.
 * @return 0  If the string was successfully reversed.
 * @return -1 If the input pointer is NULL.
 *
 * @note The function assumes that the string is properly null-terminated.
 *
 */
int ReverseStr(char* str);

/**
 * Check if a string is a palindrome.
 *
 * @param str Pointer to a null-terminated string.
 *
 * @return 1 if palindrome.
 * @return 0 if not palindrome.
 * @return -1 if str is NULL.
 */
int IsPalindrome(const char* str);


/**
 * Converts a numeric string to an integer value.
 *
 * This function parses a null-terminated string and converts the leading
 * numeric portion into an integer value. The parsed result is written into
 * the variable pointed to by `number`.
 *
 * Behavior:
 * - Leading whitespace characters are ignored.
 * - A single optional sign character ('+' or '-') is supported.
 * - Conversion stops when the first non-digit character is encountered.
 * - If no valid digits are found, the result will be 0.
 *
 * Supported whitespace characters:
 * ' ', '\t', '\n', '\r', '\f', '\v'
 *
 * @param str
 * Pointer to a null-terminated input string to convert.
 *
 * @param number
 * Pointer to an integer where the parsed result will be stored.
 *
 * @return
 * 0  - Success.
 * -1 - Invalid input (NULL pointer provided).
 */
int MyAToI(char* str,int* number);



/*
 * Converts an integer value to its string representation.
 *
 * Parameters:
 *  number  - The integer value to convert.
 *  buffer  - Pointer to a character array where the resulting string
 *            will be stored. The buffer must be large enough to hold
 *            the resulting string including the null terminator.
 *            For a 32-bit int, a buffer size of at least 12 bytes is recommended.
 *
 * Return value:
 *  0  - Conversion succeeded.
 * -1  - Invalid argument (buffer is NULL).
 *
 * Notes:
 *  - The resulting string is null-terminated.
 *  - Negative numbers are supported and will include the '-' sign.
 *  - The function assumes that the provided buffer has sufficient space.
 */
int MyToA(int number,char* buffer);


/* Converts an integer value to its string representation.
 *
 * This version builds the digits in reverse order and then
 * reverses the resulting string using a string reverse utility.
 *
 * Parameters:
 *  number  - The integer value to convert.
 *  buffer  - Pointer to a character array where the resulting
 *            string will be stored. The buffer must be large
 *            enough to hold the resulting string including the
 *            null terminator. For a 32-bit int, a buffer size of
 *            at least 12 bytes is recommended.
 *
 * Return value:
 *  0  - Conversion succeeded.
 * -1  - Invalid argument (buffer is NULL).
 *
 * Notes:
 *  - The resulting string is null-terminated.
 *  - Negative numbers are supported.
 *  - This implementation does not handle INT_MIN due to integer
 *    overflow when converting the value to a positive number.
 */
int MyToA_V2(int number,char* buffer); 



/**
 * Reverses the order of words in a null-terminated string.
 *
 * The function reverses the order of words in the given string in-place.
 * Words are sequences of non-space characters separated by spaces.
 * The operation is performed without allocating additional memory.
 *
 * @param str
 * Pointer to a null-terminated string to be modified.
 *
 * @return
 * 0 on success.
 * -1 if the input pointer is NULL.
 *
 * @note
 * The function modifies the original string.
 */
int ReverseWordsInString(char* str);



/**
 * Counts the number of words in a null-terminated string.
 *
 * A word is defined as a sequence of non-space characters separated by
 * whitespace characters (such as space, tab, newline, etc.).
 *
 * @param str
 * Pointer to a null-terminated string to be analyzed.
 *
 * @return
 * - Number of words in the string on success.
 * - -1 if the input pointer is NULL.
 *
 * @note
 * The function does not modify the input string.
 */
int CountWordsInString(char* str);

/**
 * Returns non-zero if the character is a whitespace character.
 *
 * @param c  Character to check.
 * @return Non-zero if whitespace, otherwise 0.
 */
int IsWhiteSpace(char c);


#endif