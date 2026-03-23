#include"wc.h"

int main()
{
    const char* filePath = "./test_wc.txt";
    FILE *fp = fopen(filePath, "w");
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

    int words = 0;
    int chars = 0;
    int lines = 0;
    
    int ret = wc_function(filePath, &words, &chars, &lines);

    if (ret != 0)
    {
        printf("wc failed\n");
        return -1;
    }

    printf("Result from my wc function:\n");
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Chars: %d\n", chars);

    printf("\nCompare with Linux wc:\n");
    char command[256];
    snprintf(command, sizeof(command), "wc %s", filePath);
    system(command);

    return 0;
}