#include "tail.h"

#define TAIL_LINES 5
int main()
{
    FILE *fp = fopen("./test.txt", "w");
    if (fp == NULL)
    {
        printf("FAIL to create test file\n");
        return -1;
    }

    for (int i = 0; i < 100; ++i)
    {
        fprintf(fp, "line %d\n", i);
    }

    fclose(fp);

    int linesToPrint = TAIL_LINES;
    char *lines[TAIL_LINES];
    int linesAdded = 0;
    int startIndex = 0;
    int index = 0;

    if (Circular_Buffer_Tail("./test.txt", linesToPrint, lines, &linesAdded, &startIndex) != 0)
    {
        printf("Tail failed\n");
        return -1;
    }

    for (int i = 0; i < linesAdded; ++i)
    {
        index = (startIndex + i) % linesToPrint;
        printf("%s", lines[index]);
    }

    for (int i = 0; i < linesToPrint; ++i)
    {
        free(lines[i]);
        lines[i] = NULL;
    }

    return 0;
}