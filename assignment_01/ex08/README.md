# Armstrong Numbers (0–999)
Description

This program checks whether a number is an 3digits Armstrong number and prints all Armstrong numbers in the range 0–999.


An Armstrong number is an integer that is equal to the sum of its digits each raised to the power of the number of digits.

Example:

371 is an Armstrong number because:

3³ + 7³ + 1³ = 371

Program Structure

The program is implemented using the following functions:

CountIntDigits(int number)
Counts the number of digits in the given integer.

PowInt(int base, int exp)
Computes the power of an integer without using floating-point math.

IsArmstrong(int number)
Determines whether a given number is an Armstrong number.

The main() function iterates through numbers from 100 to 999 and prints the Armstrong numbers found.