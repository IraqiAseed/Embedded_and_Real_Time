#include <stdio.h>
#define FACT_SUCCESS 0
#define FACT_ERR_NULLPTR 1
#define FACT_ERR_NEGATIVE 2
#define FACT_ERR_OVERFLOW 3
#define FACT_ERR_BADTYPE 4
#define FACT_MAX_N 12

typedef enum
{
    FACT_RECURSIVE,
    FACT_ITERATIVE,
    FACT_TYPE_COUNT
} FactType;

int RecursiveFactorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    return n * RecursiveFactorial(n - 1);
}

int IterativeFactorial(int n)
{
    int factorial = 1;

    while (n > 1)
    {
        factorial *= n;
        --n;
    }
    return factorial;
}

int Factorial(int n, FactType type, int *result)
{
    int ret = FACT_SUCCESS;

    if (result == NULL)
    {
        ret = FACT_ERR_NULLPTR;
    }
    else if (n < 0)
    {
        ret = FACT_ERR_NEGATIVE;
    }
    else if (n > FACT_MAX_N)
    {
        ret = FACT_ERR_OVERFLOW;
    }
    else if (type == FACT_RECURSIVE)
    {
        *result = RecursiveFactorial(n);
    }
    else if (type == FACT_ITERATIVE)
    {
        *result = IterativeFactorial(n);
    }
    else
    {
        ret = FACT_ERR_BADTYPE;
    }
    return ret;
}

const char *FactorialErrorToString(int code)
{
    switch (code)
    {
    case FACT_SUCCESS:
        return "Success";
    case FACT_ERR_NULLPTR:
        return "Null pointer error";
    case FACT_ERR_NEGATIVE:
        return "Negative input error";
    case FACT_ERR_OVERFLOW:
        return "Overflow error";
    case FACT_ERR_BADTYPE:
        return "Invalid type error";
    default:
        return "Unknown error";
    }
}
const char *FactTypeToString(FactType type)
{
    switch (type)
    {
    case FACT_RECURSIVE:
        return "Recursive";
    case FACT_ITERATIVE:
        return "Iterative";
    default:
        return "Unknown";
    }
}
int main(void)
{
    int a;
    int res;
    int status;

    printf("=== Factorial Iterative && Recursive ===\n");
    printf("Enter a non-negative integer (0..12): ");

    if (scanf("%d", &a) != 1)
    {
        printf("Invalid input.\n");
        return 1;
    }

    for (FactType type = FACT_RECURSIVE; type < FACT_TYPE_COUNT; ++type)
    {
        status = Factorial(a, type, &res);

        if (status == FACT_SUCCESS)
        {
            printf("[%s] %d! = %d\n", FactTypeToString(type), a, res);
        }
        else
        {
            printf("%s\n", FactorialErrorToString(status));
        }
    }

    return 0;
}