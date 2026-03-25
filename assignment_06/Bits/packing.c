#include "packing.h"
#include <stdbool.h>

int validate_string(char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    while (*str != '\0')
    {

        if (*str < 'a' || *str > 'o')
        {
            printf("Invalid character found: %c - language is not supported\n", *str);
            return -1;
        }

        str++;
    }

    return 0;
}
int compress_string_4bit_bitwise(char *str)
{

    if (validate_string(str) == -1)
    {
        return -1;
    }

    int packed_value = 0;
    int read_index = 0;
    int write_index = 0;
    bool left = true;

    while (str[read_index] != '\0')
    {
        packed_value = str[read_index] - 'a'; // reads bytes and converts to 0-15

        if (left == true)
        {
            str[write_index] = (packed_value << 4);
            left = false;
        }
        else
        {
            str[write_index] |= packed_value;
            write_index++;
            left = true;
        }

        read_index++;
    }

    if (left == false)
    {
        str[write_index] |= 0X0F;
        write_index++;
    }

    str[write_index] = 0xFF; // End marker for the packed data
                             // since we are using 4 bits to store characters,
                             // we can use 0xFF to indicate the end of the string

    return write_index;
}



int compress_string_4bit_bitfield(char *str)
{
     if (validate_string(str) == -1)
    {
        return -1;
    }
    
    union PackedByte packed;
    
    int packed_value = 0;
    int read_index = 0;
    int write_index = 0;
    bool left = true;

    while (str[read_index] != '\0')
    {
        packed_value = str[read_index] - 'a'; // reads bytes and converts to 0-15

        if (left == true)
        {
            packed.byte = 0;
            packed.bits.high = packed_value;
            left = false;
        }
        else
        {
            packed.bits.low = packed_value;
            str[write_index] = packed.byte;
            
            write_index++;
            left = true;
        }

        read_index++;
    }

    if (left == false)
    {
        packed.bits.low = 0x0F; //modify only low bits, high bits are already set
        str[write_index] = packed.byte;
        write_index++;
    }

    str[write_index] = 0xFF; // End marker for the packed data
                             // since we are using 4 bits to store characters,
                             // we can use 0xFF to indicate the end of the string

    return write_index;
}

void print_compressed_string(char *str)
{
    if(str == NULL)
    {
        return;
    }
    
    int i = 0;

    while ((unsigned char)str[i] != 0xFF)
    {
        unsigned char byte = (unsigned char)str[i];

        int high = (byte >> 4) & 0xF;
        int low = byte & 0xF;

        printf("%X ", high);

        if (low != 0x0F)
        {
            printf("%X ", low);
        }

        i++;
    }
    printf("\n");
}