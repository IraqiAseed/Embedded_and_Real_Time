#include <stdio.h>

#define CALC_SUCCESS 0
#define CALC_ERROR 1

int Calculate(int a, int b, char op, int *result)
{
    int ret = CALC_SUCCESS;

    if (result == NULL)
        return CALC_ERROR;

    switch (op)
    {
    case '+':
        *result = a + b;
        break;

    case '-':
        *result = a - b;
        break;

    case '*':
        *result = a * b;
        break;

    default:
        ret = CALC_ERROR; /* operation is not supported */
        break;
    }

    return ret;
}
void PrintResult(int a, int b, char op, int result)
{
    switch (op)
    {
    case '+':
        printf("The sum Of the 2 numbers (%d+%d)= %d\n", a, b, result);
        break;

    case '-':
        printf("The difference between the 2 numbers (%d-%d)= %d\n", a, b, result);
        break;

    case '*':
        printf("The multiply Of the 2 numbers (%d*%d)= %d\n", a, b, result);
        break;

    default:
        printf("Operation not supported\n");
        break;
    }
}

int main()
{
    int a, b, res, i;

    char ops[] = {'+', '-', '*'};
    int count = sizeof(ops) / sizeof(ops[0]);
    printf("===== Simple Calculator =====\n");
    printf("This program performs +, -, * operations on two numbers\n");

    printf("Enter first number: ");
    scanf("%d", &a);

    printf("Enter second number: ");
    scanf("%d", &b);

    for (i = 0; i < count; i++)
    {
        if (Calculate(a, b, ops[i], &res) == CALC_SUCCESS)
        {
            PrintResult(a, b, ops[i], res);
        }
    }

    return 0;
}