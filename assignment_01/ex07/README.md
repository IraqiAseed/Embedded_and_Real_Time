# DigitsCheck

Write a function which gets 2 positive integer numbers:
- The first is the **base** number.
- The second is the **check** number.

The function checks:
1. How many digits in the **check** number are in the **base** in the **same place**.
2. How many digits are in the **base** but **not in the same place**.

Notes:
- The numbers should have **different digits** (no repeated digits in each number).
- The function returns an integer number:
  - The **tens digit** contains the count of digits that are in the **same place**.
  - The **ones digit** contains the count of digits that are **not in the same place**.
- In case of error, the function returns **-1**.

Examples:
- base = 1234, check = 4321 → return 4
- base = 2468, check = 2648 → return 22