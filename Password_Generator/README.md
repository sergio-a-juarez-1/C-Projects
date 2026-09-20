# CLI-Password Generator

A lightweight, high-performance command-line password generator written in C. This project leverages low-level array manipulation and dynamic heap memory management.

---

## Key Features

- **Customized Composition:** Choose the exact quantity of letters, special symbols, and numbers required for your credentials.
- **Dynamic Case Flipping:** Implements a balanced coin-flip algorithmic process to automatically randomize uppercase and lowercase letters.
- **Fisher-Yates Shuffle Execution:** Employs an explicit inplace Fisher-Yates array shuffling loop to guarantee true layout unpredictability without structural data collision or loss.
- **Cross-Platform Compatibility:** Fully compatible with both Windows and Unix-based (`gcc`/`clang`) compiler Toolchains.

---

## 🎮 How It Works & Example Run

The application dynamically requests your parameters step-by-step to generate a custom-tailored password block:

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

---

## 🛠️ Build and Local Deployment

To compile and run this program, you will need a standard C compiler (GCC, Clang, or MinGW) installed on your system.

### 1. Isolate the Project via Sparse-Checkout
To deploy this specific utility without pulling down your entire monorepo, initialize an empty directory and pull down only the target folder:

```bash
# Initialize an empty local repository
mkdir Password_Generator && cd Password_Generator
git init

# Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and target the specific application folder
git sparse-checkout set Password_Generator

# Pull down only that folder's files
git pull origin main
```

### 2. Compilation
Compile the source file using GCC:
```bash
gcc password_generator.c -o password_generator
```

### 3. Running the Application
Execute the compiled binary from your command terminal:

```bash
./password_generator
```

---

## 📂 Deep Dive Under the Hood

The C execution workflow handles raw string manipulation directly in memory:
1. **Dynamic Memory Allocation:** It aggregates user input variables up-front to provision a perfectly sized block of memory on the heap via `malloc()`.
2. **ASCII Pointer Arithmetic:** Capitalization is calculated mathematically by subtracting `32` from lowercase ASCII values, saving system overhead.
3. **Fisher-Yates Shuffle:** Rearranges the characters by stepping backward through the memory buffer and swapping indices inline using a temporary register holder.
