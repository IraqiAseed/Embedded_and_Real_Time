#include "wc.h"

int wc_function(const char *path, int *word_count, int *char_count, int *line_count)
{
    if (path == NULL || word_count == NULL || char_count == NULL || line_count == NULL)
    {
        return -1;
    }

    *word_count = 0;
    *char_count = 0;
    *line_count = 0;

    int c = 0;
    int inWord = 0;

    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        return -1;
    }

    while ((c = fgetc(fp)) != EOF)
    {

        (*char_count)++;

        if (c == '\n')
        {
            (*line_count)++;
            inWord = 0;
        }
        else if (isspace((unsigned char)c))
        {
            inWord = 0;
        }
        else if (inWord == 0)
        {
            inWord = 1;
            (*word_count)++;
        }
    }

    if (ferror(fp))
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}
