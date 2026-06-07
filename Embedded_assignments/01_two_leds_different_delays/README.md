# Assignment 01 - Two LEDs with Different Delays

## Goal
Blink two LEDs independently:
- Blue LED every 500 ms
- Red LED every 300 ms

## What I learned
- GPIO output control
- Non-blocking delay logic using counters
- Handling multiple periodic tasks in the main loop
- Avoiding blocking HAL_Delay for each LED separately

## Main idea
Instead of using separate blocking delays, the program uses a base tick/counter and toggles each LED when its own period is reached.