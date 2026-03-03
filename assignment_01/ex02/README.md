# Exercise 02 – Factorial

This program implements a factorial function in C.

It calculates n! using both recursive and iterative approaches.

The program validates input and and handles error cases such as:
- NULL pointer
- Negative input
- Overflow (limited to 32-bit int)

## Limitations:

Since int is 32-bit, the maximum supported value is:

12! = 479001600  (valid)
13! = 6227020800 (overflow)

Therefore, the maximum allowed input is 12.
