#include <stdio.h>
#include "triangle.h"

int main()
{
    int n;
    char c;

    printf("Enter number of lines: ");
    scanf("%d", &n);

    printf("Enter character: ");
    scanf(" %c", &c);

    printf("func1:\n");
    func1(n, c);
    printf("\n");

    printf("func2:\n");
    func2(n, c);
    printf("\n");

    printf("func3:\n");
    func3(n, c);
    printf("\n");

    printf("func4:\n");
    func4(n, c);
    printf("\n");

    printf("func5:\n");
    func5(n, c);
    printf("\n");

    printf("func6:\n");
    func6(n, c);
    printf("\n");

    return 0;
}