# Tip Calculator in C

A clean, terminal-based **interactive bill splitter and tip calculator** written in C. The program takes a subtotal, prompts for a specific tip percentage, and calculates the exact split amount for a group of people.

## Features

- **Interactive CLI:** Guides you step-by-step through user inputs.
- **Precise Math:** Uses floating-point math (`double`) to guarantee accuracy down to the penny.
- **Formatted Output:** Strips long floats down to a standard currency format (`$%.2f`).

## Getting Started

### Prerequisites

You need a standard C compiler such as **GCC** or **Clang** installed on your operating system.

### Compilation

Open your terminal in the directory containing `main.c` and compile using the following command:

```bash
gcc main.c -o tip_calculator
```

### Running the Application

Execute the compiled binary:

```bash
./tip_calculator
```

## Example Usage

```text
Welcome to the tip calculator.
What was the total bill? $124.50
What percentage tip would you like to give? 10, 12, or 15? 12
How many people to split the bill? 3
Each person should pay: $46.48
```

## Code Logic Breakdown

1. **Input Collection:** Uses `scanf` to securely ingest the `double` total bill, `int` tip percentage, and `int` party split size.
2. **Tip Math:** Converts the integer percentage into a fractional multiplier (`percent_tip / 100.0`) to avoid integer division truncation.
3. **Distribution:** Divides the global total uniformly by the number of participants.
