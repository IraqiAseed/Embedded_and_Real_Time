#include "tail.h"


int Circular_Buffer_Tail(const char *path, int numOfLines, char **lines, int *linesAdded, int *startIndex)
{
    if (path == NULL || numOfLines <= 0 || lines == NULL || linesAdded == NULL || startIndex == NULL)
    {
        return -1;
    }

    *linesAdded = 0;
    *startIndex = 0;

    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        return -1;
    }

    char buffer[4096];  //max line length is 4k
    int nextIndex = 0;
    int count = 0;

    for (int i = 0; i < numOfLines; ++i)
    {
        lines[i] = NULL;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);

        char *newLine = malloc(len + 1);
        
        // failed to allocate new memory, delete existing memory
        if (newLine == NULL)
        {
            for (int i = 0; i < numOfLines; ++i)
            {
                free(lines[i]);
                lines[i] = NULL;
            }

            fclose(fp);
            return -1;
        }

        strcpy(newLine, buffer);

        if (lines[nextIndex] != NULL)
        {
            free(lines[nextIndex]);
        }

        lines[nextIndex] = newLine;

        nextIndex = (nextIndex + 1) % numOfLines;

        if (count < numOfLines)
        {
            count++;
        }
    }

    *linesAdded = count;

    if (count == numOfLines)
    {
        *startIndex = nextIndex;
    }
 

    fclose(fp);
    return 0;
}