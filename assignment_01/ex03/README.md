preventing integer overflow when reverse an integer
rev = rev * 10 + digit

## INT_MAX overflow
--------------------
rev * 10 + digit ≤ INT_MAX

rev * 10 ≤ INT_MAX − digit
rev ≤ (INT_MAX − digit) / 10

Let:
M = INT_MAX
q = M / 10
r = M % 10
Then:
M = 10q + r

Case 1 — rev < q

Since rev and q are integers:

rev < q

implies:

rev ≤ q − 1

Multiply both sides by 10:

rev * 10 ≤ (q − 1) * 10

Expand:

(q − 1) * 10 = 10q − 10

So:
rev * 10 ≤ 10q − 10

Now add the largest possible digit (9):

rev * 10 + digit ≤ 10q − 10 + 9
rev * 10 + digit ≤ 10q − 1

Since:

10q − 1 < 10q + r = INT_MAX

there is no overflow possible.

Case 2 — rev > q

If:

rev > q

then:

rev ≥ q + 1

Multiply by 10:

rev * 10 ≥ (q + 1) * 10
rev * 10 ≥ 10q + 10

Even with the smallest digit:

rev * 10 + digit ≥ 10q + 10

Since:

10q + 10 > 10q + r = INT_MAX

overflow must occur.

Case 3 — rev == q (boundary case)

Here:

rev * 10 = 10q

So:

rev * 10 + digit = 10q + digit

To avoid overflow:

10q + digit ≤ 10q + r

Subtract 10q:

digit ≤ r

Which means:

digit ≤ INT_MAX % 10
Final Overflow Check

Combining the cases above:

overflow if:

rev > INT_MAX / 10
OR
rev == INT_MAX / 10 AND digit > INT_MAX % 10

Implementation in C:

if (rev > INT_MAX / 10 ||
    (rev == INT_MAX / 10 && digit > INT_MAX % 10))
{
    return REV_OVERFLOW;
}


## INT_MIN overflow
--------------------
rev * 10 + digit ≥ INT_MIN

Subtract digit from both sides:

rev * 10 ≥ INT_MIN − digit

Divide by 10:

rev ≥ (INT_MIN − digit) / 10
Step 2 — Express INT_MIN using division

Let:

m = INT_MIN
q = m / 10
r = m % 10

Then:

m = 10q + r
Case 1 — rev > q

If:

rev > q

then:

rev ≥ q + 1

Multiply both sides by 10:

rev * 10 ≥ (q + 1) * 10
rev * 10 ≥ 10q + 10

Now add the smallest possible digit (-9):

rev * 10 + digit ≥ 10q + 10 - 9
rev * 10 + digit ≥ 10q + 1

Since:

10q + 1 > 10q + r = INT_MIN

the result is still within range, so no overflow occurs.

Case 2 — rev < q

If:

rev < q

then:

rev ≤ q − 1

Multiply by 10:

rev * 10 ≤ (q − 1) * 10
rev * 10 ≤ 10q − 10

Even if digit = 0:

rev * 10 + digit ≤ 10q − 10

Since:

10q − 10 < 10q + r = INT_MIN

this results in overflow.

Case 3 — Boundary case (rev == q)

Here:

rev * 10 = 10q

So:

rev * 10 + digit = 10q + digit

To remain within bounds we need:

10q + digit ≥ 10q + r

Subtract 10q:

digit ≥ r

Which means:

digit ≥ INT_MIN % 10

If:

digit < r

then overflow occurs.

Final Negative Overflow Check

Combine the conditions:

overflow if:

rev < INT_MIN / 10
OR
rev == INT_MIN / 10 AND digit < INT_MIN % 10