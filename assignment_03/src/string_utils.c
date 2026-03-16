#include <stdio.h>
#include <string.h>
#include "../include/string_utils.h"

int ReverseStr(char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    int length = strlen(str);
    if (length == 0)
    {
        return 0;
    }
    int right = length - 1;
    ;
    int left = 0;
    char tmp;

    while (right > left)
    {
        tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
        right--;
        left++;
    }

    return 0;
}

int IsPalindrome(const char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    int length = strlen(str);
    int right = length - 1;
    int left = 0;

    while (right > left)
    {
        if (str[right] != str[left])
        {
            return 0;
        }

        left++;
        right--;
    }
    return 1;
}

int MyAToI(char *str, int *number)
{
    if (str == NULL || number == NULL)
    {
        return -1;
    }

    *number = 0;
    int sign_seen = 0;
    int digit;
    int sign_factor = 1;
    int found_digit = 0;
    ;

    while (*str != '\0')
    {
        if (IsWhiteSpace(*str))
        {
            if (found_digit == 0)
            {
                str++;
                continue;
            }
            else
            {
                break;
            }
        }
        if ((*str == '+' || *str == '-') && sign_seen == 1)
        {
            break;
        }
        if ((*str == '+' || *str == '-') && sign_seen == 0 && found_digit == 0)
        {
            sign_seen = 1;

            if (*str == '-')
            {
                sign_factor = -1;
            }

            if ((*(str + 1) < '0' || *(str + 1) > '9'))
            {
                return 0;
            }
            str++;
            continue;
        }

        if (*str >= '0' && *str <= '9')
        {
            found_digit = 1;
            digit = *str - '0';
            *number = *number * 10 + digit;
        }
        else
        {
            break;
        }

        str++;
    }

    *number *= sign_factor;

    return 0;
}

int MyToA_V2(int number, char *buffer)
{
    if (buffer == NULL)
    {
        return -1;
    }

    int digit = 0;
    int i = 0;
    int sign = 1;

    if (number == 0)
    {
        buffer[i] = '0';
        i++;
    }
    if (number < 0)
    {
        sign = -1;
        number *= -1;
    }

    while (number)
    {
        digit = number % 10;
        buffer[i] = digit + '0';
        number /= 10;
        i++;
    }

    if (sign == -1)
    {
        buffer[i] = '-';
        i++;
    }

    buffer[i] = '\0';

    return ReverseStr(buffer);
}

int MyToA(int number, char *buffer)
{
    if (buffer == NULL)
    {
        return -1;
    }

    if (number == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 0;
    }

    int i = 0;
    int negative = 0;
    int count_digits = 0;
    // using long long to handle INT_MIN, INT_MIN * -1 will be bigger than INT_MAX
    long long tmp = number;
    long long tmp2 = 0;

    if (tmp < 0)
    {
        negative = 1;
        tmp *= -1;
    }

    tmp2 = tmp;

    while (tmp)
    {
        tmp /= 10;
        count_digits++;
    }

    count_digits += negative; // add one extra position for '-' if number is negative
    buffer[count_digits] = '\0';
    i = count_digits - 1;

    while (tmp2)
    {
        buffer[i] = tmp2 % 10 + '0';
        tmp2 /= 10;
        i--;
    }

    if (negative)
    {
        buffer[0] = '-';
    }

    return 0;
}

// indexes version
int ReverseWordsInString(char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    int start = 0; // start of word index
    int end = 0;   // end of word index
    int word_length = 0;
    char tmp;
    int next_index = 0;
    char *p = str;

    ReverseStr(str);

    while (*p != '\0')
    {
        while (*p == ' ')
        {
            p++;
            start++;
        }
        while (*p != '\0' && *p != ' ')
        {
            p++;
            word_length++;
        }

        end = start + word_length - 1;
        next_index = end + 1;

        while (start < end)
        {
            tmp = str[end];
            str[end] = str[start];
            str[start] = tmp;
            start++;
            end--;
        }

        start = next_index;
        word_length = 0;
    }

    return 0;
}

// pointer version
int ReverseWordsInString_pointerVersion(char *str)
{
    if (str == NULL)
        return -1;

    ReverseStr(str);

    char *p = str;

    while (*p)
    {
        while (*p == ' ')
            p++;

        char *start = p;

        while (*p && *p != ' ')
            p++;

        char *end = p - 1;

        while (start < end)
        {
            char tmp = *start;
            *start = *end;
            *end = tmp;
            start++;
            end--;
        }
    }

    return 0;
}
int IsWhiteSpace(char c)
{
    return (c == ' '  ||
            c == '\t' ||
            c == '\n' ||
            c == '\r' ||
            c == '\v' ||
            c == '\f');
}
int CountWordsInString(char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    int in_word = 0;
    int counter = 0;

    while (*str)
    {
        if (IsWhiteSpace(*str))
        {
            in_word = 0;
        }
        else if (in_word == 0)
        {
            counter++;
            in_word = 1;
        }
        str++;
    }

    return counter;
}