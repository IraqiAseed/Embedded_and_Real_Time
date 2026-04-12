#include <stdio.h>
#include <string.h>

static void reverse_string(char *str, int left, int right)
{
    if (left >= right)
    {
        return;
    }

    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;

    reverse_string(str, left + 1, right - 1);
}

static void is_palindrome(const char *str, int left, int right, int *result)
{
    if (left >= right)
    {
        *result = 1;
        return;
    }

    if (str[left] != str[right])
    {
        *result = 0;
        return;
    }

    is_palindrome(str, left + 1, right - 1, result);
}

static double average_recursive(const int arr[], size_t size)
{
    if (size == 1)
    {
        return arr[0];
    }

    return ((average_recursive(arr, size - 1) * (size - 1)) +
            arr[size - 1]) /
           size;
}

static void hanoi(int n, char source, char helper, char destination)
{
    if (n == 1)
    {
        printf("move disk 1 from %c to %c\n", source, destination);
        return;
    }

    hanoi(n - 1, source, destination, helper);

    printf("move disk %d from %c to %c\n", n, source, destination);

    hanoi(n - 1, helper, source, destination);
}

int main()
{
    char str[] = "hello";
    printf("Original string: %s\n", str);
    reverse_string(str, 0, strlen(str) - 1);
    printf("Reversed string: %s\n", str);

    char palindrome_str1[] = "madam";
    int result = 0;
    is_palindrome(palindrome_str1, 0, strlen(palindrome_str1) - 1, &result);
    printf("Is '%s' a palindrome? %s\n", palindrome_str1, result ? "Yes" : "No");
    char palindrome_str2[] = "hello";

    is_palindrome(palindrome_str2, 0, strlen(palindrome_str2) - 1, &result);
    printf("Is '%s' a palindrome? %s\n", palindrome_str2, result ? "Yes" : "No");

    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("Average of the array is: %.2f\n", average_recursive(arr, 10));

    printf("Tower of Hanoi with 3 disks:\n");
    hanoi(3, 'A', 'B', 'C');

    return 0;
}