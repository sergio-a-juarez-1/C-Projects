# CLI-Password Generator (C Port)

A lightweight, high-performance command-line password generator written in C. This project is a direct, optimized port of a classic Python password-building script, adapted to leverage low-level array manipulation and dynamic heap memory management.

## Features

- **Customized Composition**: Choose the exact quantity of letters, special symbols, and numbers required for your credentials.
- **Dynamic Case Flipping**: Implements a balanced coin-flip algorithmic process to automatically randomize uppercase and lowercase letters.
- **Fisher-Yates Shuffle Execution**: Employs an explicit inplace Fisher-Yates array shuffling loop to guarantee true layout unpredictability without structural data collision or loss.
- **Cross-Platform Compatibility**: Fully compatible with both Windows and Unix-based (`gcc`/`clang`) compiler Toolchains.

## Prerequisites

To compile and run this program, you will need a standard C compiler installed on your system:
- **Linux/macOS**: `gcc` or `clang`
- **Windows**: MinGW, MSVC, or Clang

## Installation & Compilation

1. Clone the repository to your local directory:
   ```bash
   git clone https://github.com/sergio-a-juarez-1/C-Projects.git
   ```
2. Navigate into the project folder:
   ```bash
   cd Rock-Paper-Scissors
   ```
3. Compile the source file using GCC:
   ```bash
   gcc password_generator.c -o password_generator
   ```

## Usage

Run the compiled executable from your command terminal:

```bash
./password_generator
```

### Example Run

```text
Welcome to the CLI-Password Generator!
How many letters would you like in your password?
4
How many symbols would you like?
5
How many numbers would you like?
5
Here is your password: !8v*8O5i%b0#&7
```

## How It Works Under the Hood

Unlike Python's abstracted container systems, the C execution workflow handles raw string manipulation directly in memory:
1. **Dynamic Memory Allocation**: It aggregates user input variables up-front to provision a perfectly sized block of memory on the heap via `malloc()`.
2. **ASCII Pointer Arithmetic**: Capitalization is calculated mathematically by subtracting `32` from lowercase ASCII values, saving system overhead.
3. **Fisher-Yates Shuffle**: Rearranges the characters by stepping backward through the memory buffer and swapping indices inline using a temporary register holder.
