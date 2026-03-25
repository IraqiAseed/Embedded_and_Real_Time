#include "packing.h"
#include <string.h>

int main()
{
    char original[] = "badmoon";
    char str_bitwise[sizeof(original)];
    char str_bitfield[sizeof(original)];

    memcpy(str_bitwise, original, sizeof(original));
    memcpy(str_bitfield, original, sizeof(original));

    printf("=== Exercise Packing Table ===\n");
    for (char c = 'a'; c <= 'o'; c++)
    {
        printf("%c -> ASCII: %d, Packed: %d\n", c, c, c - 'a');
    }

    printf("\nOriginal string: [%s]\n\n", original);

    int length_bitwise = compress_string_4bit_bitwise(str_bitwise);
    if (length_bitwise == -1)
    {
        printf("Bitwise compression failed\n");
        return -1;
    }

    printf("Bitwise  : ");
    print_compressed_string(str_bitwise);

    int length_bitfield = compress_string_4bit_bitfield(str_bitfield);
    if (length_bitfield == -1)
    {
        printf("Bitfield compression failed\n");
        return -1;
    }

    printf("Bitfield : ");
    print_compressed_string(str_bitfield);

    return 0;
}