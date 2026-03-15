#include "../include/string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void test_palindrome(char str[])
{
    int result = IsPalindrome(str);

    if (result == -1)
    {
        printf("Input error\n");
    }
    else if (result == 1)
    {
        printf("%s is palindrome\n", str);
    }
    else
    {
        printf("%s is not palindrome\n", str);
    }

    printf("-----------------------------\n");
}

void test_reverse(char str[])
{
    printf("Original string : %s\n", str);

    if (ReverseStr(str) == 0)
    {
        printf("Reversed string : %s\n", str);
    }
    else
    {
        printf("FAILED to reverse string\n");
    }

    printf("-----------------------------\n");
}
void test_reverse_words(char *str)
{
    printf("Original: [%s]\n", str);
    ReverseWordsInString(str);
    printf("ReverseWord Result  : [%s]\n", str);
    printf("-----------------------------\n");
}

void test_myatoi(char *str)
{
    int myAtoiResult = 0;
    int expected = atoi(str);
    int status = MyAToI(str, &myAtoiResult);

    if (status != 0)
    {
        printf("FAIL: given string = \"%s\"  [MyAToI returned status %d]\n", str, status);
        printf("-----------------------------\n");
        return;
    }

    if (expected == myAtoiResult)
    {
        printf("SUCCESS: given string = \"%s\"  result = %d\n", str, myAtoiResult);
    }
    else
    {
        printf("FAIL: given string = \"%s\"  [MyAToI = %d, atoi = %d]\n",
               str, myAtoiResult, expected);
    }

    printf("-----------------------------\n");
}
void test_myitoa(int number)
{
    char myBuffer[12];
    char expectedBuffer[12];

    MyToA(number, myBuffer);
    snprintf(expectedBuffer, sizeof(expectedBuffer), "%d", number);

    if (strcmp(myBuffer, expectedBuffer) == 0)
    {
        printf("SUCCESS: given number = %d result = %s\n", number, myBuffer);
    }
    else
    {
        printf("FAIL: given number = %d  [MyToA=%s , itoa=%s]\n",
               number, myBuffer, expectedBuffer);
    }

    printf("-----------------------------\n");
}

void run_reverse_tests()
{
    // reverse tests
    char rev1[] = "abcd";
    char rev2[] = "hello";
    char rev3[] = "a";
    char rev4[] = "";

    printf("Reverse tests\n");
    printf("=============================\n");
    test_reverse(rev1);
    test_reverse(rev2);
    test_reverse(rev3);
    test_reverse(rev4);
}

void run_palindrome_tests()
{

    char pal1[] = "a";
    char pal2[] = "abba";
    char pal3[] = "abcba";
    char pal4[] = "abcd";
    char pal5[] = "";
    char pal6[] = "ab";

    printf("\nPalindrome tests\n");
    printf("=============================\n");
    test_palindrome(pal1);
    test_palindrome(pal2);
    test_palindrome(pal3);
    test_palindrome(pal4);
    test_palindrome(pal5);
    test_palindrome(pal6);
}

void run_atoi_tests()
{
    char num1[] = "123";
    char num2[] = "-123";
    char num3[] = "    123  55";
    char num4[] = "3.14";
    char num5[] = "+44";
    char num6[] = "-8 44";
    char num7[] = "67abc";
    char num8[] = "abc987";
    char num9[] = "abc 9 8 7";
    char num10[] = "-";
    char num11[] = "-9";
    char num12[] = "-0";
    char num13[] = "12-3";
    char num14[] = "\t123";
    char num15[] = "\n-45";
    char num16[] = "\v+78";
    char num17[] = "+abc";
    char num18[] = "   -abc";
    char num19[] = "0";
    char num20[] = "000123";
    char num21[] = "-00045";
    char num22[] = "99+7";
    char num23[] = "--12";
    char num24[] = "++12";
    char num25[] = "+-12";
    char num26[] = "-+12";
    char num27[] = "- 12";
    char num28[] = "+ 34";
    char num29[] = "   +0";
    char num30[] = "   000";
    char num31[] = " ";
    char num32[] = "";

    printf("\nMy Atoi tests\n");
    printf("=============================\n");
    test_myatoi(num1);
    test_myatoi(num2);
    test_myatoi(num3);
    test_myatoi(num4);
    test_myatoi(num5);
    test_myatoi(num6);
    test_myatoi(num7);
    test_myatoi(num8);
    test_myatoi(num9);
    test_myatoi(num10);
    test_myatoi(num11);
    test_myatoi(num12);
    test_myatoi(num13);
    test_myatoi(num14);
    test_myatoi(num15);
    test_myatoi(num16);
    test_myatoi(num17);
    test_myatoi(num18);
    test_myatoi(num19);
    test_myatoi(num20);
    test_myatoi(num21);
    test_myatoi(num22);
    test_myatoi(num23);
    test_myatoi(num24);
    test_myatoi(num25);
    test_myatoi(num26);
    test_myatoi(num27);
    test_myatoi(num28);
    test_myatoi(num29);
    test_myatoi(num30);
    test_myatoi(num31);
    test_myatoi(num32);
}

void run_itoa_tests()
{
    printf("\nMy Itoa tests\n");
    printf("=============================\n");

    test_myitoa(123);
    test_myitoa(-123);
    test_myitoa(0);
    test_myitoa(7);
    test_myitoa(-8);
    test_myitoa(1000);
    test_myitoa(-450);
    test_myitoa(10);
    test_myitoa(-10);
    test_myitoa(999);
    test_myitoa(-999);

    test_myitoa(INT_MAX); // test int limits
    test_myitoa(INT_MIN);
}
void run_reverse_words_tests()
{
    char str1[] = "one two three";
    char str2[] = "hello world ";
    char str3[] = " hello world";
    char str4[] = " hello world ";
    char str5[] = "   ";
    char str6[] = "";
    char str7[] = "a";
    char str8[] = "a b";
    char str9[] = "a  b";
    test_reverse_words(str1);
    test_reverse_words(str2);
    test_reverse_words(str3);
    test_reverse_words(str4);
    test_reverse_words(str5);
    test_reverse_words(str6);
    test_reverse_words(str7);
    test_reverse_words(str8);
    test_reverse_words(str9);
}