#include<stdio.h>
#include<string.h>

/**
 * Validates that the input string contains only supported characters.
 *
 * This function checks whether the given C string is valid for the packing
 * algorithm. A valid string contains only lowercase letters in the range
 * 'a' to 'o'.
 *
 * @param str Pointer to the null-terminated input string to validate.
 *
 * @return
 * 0  if the string is valid.  
 * -1 if the string is NULL or contains unsupported characters.
 *
 * @note
 * Supported characters are:
 * 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o'
 */
int validate_string(char *str);


/**
 * Compresses a string in-place using manual bitwise operations.
 *
 * This function compresses a validated input string so that each character
 * occupies only 4 bits instead of 8. Two packed characters are stored in
 * one byte.
 *
 * Character mapping:
 * 'a' -> 0, 'b' -> 1, ..., 'o' -> 14
 *
 * Packing format:
 * - First character is stored in the high 4 bits
 * - Second character is stored in the low 4 bits
 *
 * If the number of characters is odd:
 * - The last character is stored in the high 4 bits
 * - The low 4 bits are filled with 0x0F as padding
 *
 * An end marker 0xFF is written after the packed data.
 *
 * Compression is performed in-place in the same character array.
 *
 * @param str Pointer to the null-terminated input string to compress.
 *
 * @return  Number of packed bytes written on success.  
 *          -1 on failure (for example, invalid input).
 *
 * @warning
 * After compression, the buffer no longer contains a regular text string.
 * It contains packed binary data followed by a 0xFF end marker.
 */
int compress_string_4bit_bitwise(char* str);

/**
 * Compresses a string in-place using bit fields and a union.
 *
 * This function performs the same compression as
 * compress_string_4bit_bitwise(), but uses a bit-field structure and a union
 * to build each packed byte instead of explicit bitwise operators.
 *
 * Character mapping:
 * 'a' -> 0, 'b' -> 1, ..., 'o' -> 14
 *
 * Packing format:
 * - First character is stored in the high 4 bits
 * - Second character is stored in the low 4 bits
 *
 * If the number of characters is odd:
 * - The last character is stored in the high 4 bits
 * - The low 4 bits are filled with 0x0F as padding
 *
 * An end marker 0xFF is written after the packed data.
 *
 * Compression is performed in-place in the same character array.
 *
 * @param str Pointer to the null-terminated input string to compress.
 *
 * @return
 *          Number of packed bytes written on success.  
 *          -1 on failure (for example, invalid input).
 *
 * @warning
 * Bit-field memory layout may depend on the compiler implementation.
 */
int compress_string_4bit_bitfield(char *str);

/**
 * Prints packed compressed data as hexadecimal nibble values.
 *
 * This function reads packed data produced by the compression functions
 * and prints the encoded 4-bit values in hexadecimal form until the 0xFF
 * end marker is reached.
 *
 * The function:
 * - extracts the high 4 bits
 * - extracts the low 4 bits
 * - ignores low nibble padding value 0x0F
 *
 * @param str Pointer to the packed compressed buffer terminated by 0xFF.
 *
 * @note
 * This function is intended for debugging and visualization of the packed
 */
void print_compressed_string(char *str);


struct Packed4Bits
{
    unsigned char low : 4;
    unsigned char high : 4;

    
    
};

union PackedByte
{
    struct Packed4Bits bits;
    unsigned char byte;
};