#include "triangle.h"
#include <stdio.h>

void func1(int n, char c)
{
    if (n <= 0)
        return;

    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            printf("%c", c);
        }
        printf("\n");
    }
}

void func2(int n, char c)
{
    if (n <= 0)
        return;

    for (int i = n; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            printf("%c", c);
        }
        printf("\n");
    }
}

void func3(int n, char c)
{
    if (n <= 0)
        return;

    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            printf("%c", c);
        }
        printf("\n");
    }

    for (int i = n - 1; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            printf("%c", c);
        }
        printf("\n");
    }
}

void func4(int n, char c)
{
    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 0; j < n - i; ++j)
        {
            printf(" ");
        }

        for (int j = 0; j < i; ++j)
        {
            printf("%c ", c);
        }

        printf("\n");
    }
}

void func5(int n, char c)
{
    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 0; j < i - 1; ++j)
        {
            printf(" ");
        }

        for (int j = 0; j < n - i + 1; ++j)
        {
            printf("%c ", c);
        }

        printf("\n");
    }
}

void func6(int n, char c)
{

    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 0; j < n - i; ++j)
        {
            printf(" ");
        }

        for (int j = 0; j < i; ++j)
        {
            printf("%c ", c);
        }

        printf("\n");
    }

    for (int i = n - 1; i > 0; --i)
    {
        for (int j = 0; j < n - i; ++j)
        {
            printf(" ");
        }

        for (int j = 0; j < i; ++j)
        {
            printf("%c ", c);
        }

        printf("\n");
    }
}
