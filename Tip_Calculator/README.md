# Tip Calculator in C

A clean, terminal-based **interactive bill splitter and tip calculator** written in C. The program takes a subtotal, prompts for a specific tip percentage, and calculates the exact split amount for a group of people.

## Features

- **Interactive CLI:** Guides you step-by-step through user inputs.
- **Precise Math:** Uses floating-point math (`double`) to guarantee accuracy down to the penny.
- **Formatted Output:** Strips long floats down to a standard currency format (`$%.2f`).

## Getting Started

### Prerequisites

You need a standard C compiler such as **GCC** or **Clang** installed on your operating system.

## 🚀 Getting Started & Installation

To deploy this specific program without pulling down the entire `C-Projects` monorepo, follow these steps to use **Git sparse-checkout**:

# 1. Initialize an empty local repository
mkdir Tip_Calculator && cd Tip_Calculator
git init

# 2. Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/Python-Projects.git

# 3. Enable sparse-checkout and tell Git exactly which folder you want
git sparse-checkout set Tip_Calculator

# 4. Pull down only that folder's files
git pull origin main

### Compilation

Open your terminal in the directory containing `main.c` and compile using the following command:

```bash
gcc tip_calculator.c -o tip_calculator
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
