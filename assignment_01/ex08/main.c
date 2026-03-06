#include<stdio.h>
#include<stdbool.h>
#include<math.h>

int PowInt(int base, int exp)
{
    int result = 1;

    while(exp > 0)
    {
        result *= base;
        exp--;
    }    

    return result;
}

int CountIntDigits(int number)
{
    int counter = 0;
    
    if(number == 0)
    {
        return 1;
    }

    while(number)
    {
        number /= 10;
        counter++;
    }

    return counter;
}


bool IsArmstrong(int number)
{
    if(number < 0)
    {
        return false;
    }
        

    int sum = 0;
    int digit = 0;
    int tmp_number = number;
    int digits_count = CountIntDigits(number);

    while(tmp_number)
    {
        digit = tmp_number % 10;
        sum+= PowInt(digit,digits_count);
        tmp_number /= 10;
        
    }
    return number == sum;    

}


int main()
{
    printf("Program find and print all 3-digit Armstrong numbers (100-999)\n");
    printf("================================================================\n");
    
    for(int i = 100 ; i <= 999; ++i)
    {
        if(IsArmstrong(i))
        {
            printf("%d\n",i);
        }
    }

    return 0;
}