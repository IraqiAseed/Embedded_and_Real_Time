# Prime Number Check

## note 1
if num = a * b
if both a,b > sqrt(num)
then 
a * b > sqrt(num) * sqrt(num)
but 
sqrt(num) * sqrt(num) = num

therefore
a * b > num and this is impossible because assuemed a*b = num

then:
at least one of (a or b) must be :
a <= sqrt(num) or b<=sqrt(num)


then in the code:
the idea instead of for(i = 2; i< num ; ++i)
and we know already that i<= sqrt(num) <=> i*i <= num

then it is better to write 
for(i = 2 ; i * i < num ; ++i)

and this reduce number of iteration

## note2 : 
even numbers not prime

## note 3:
The increment i += 2 is used to check only odd numbers.

Since we already verified that the number is not divisible by 2
(i.e., the number is not even), it cannot be divisible by any
other even number.

Therefore, it is sufficient to test only odd divisors.