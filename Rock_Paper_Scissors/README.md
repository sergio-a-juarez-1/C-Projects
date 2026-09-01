# Rock, Paper, Scissors (CLI)

> A lightweight, highly responsive Command Line Interface (CLI) implementation of the classic Rock, Paper, Scissors game written in pure C.

![Gameplay Preview](https://github.com/sergio-a-juarez-1/C-Projects/blob/main/Rock_Paper_Scissors/gameplay_assets/game.gif?raw=true)<!-- Optional: Real screenshot or GIF of terminal play -->


---

## About

This project is a clean **Rock, Paper, Scissors** game built to run directly inside the terminal. It features randomized computer logic using seeding, and robust input validation to handle unexpected user choices without crashing. It serves as an excellent demonstration of procedural programming, standard I/O management, and algorithmic state evaluation in C.

---

## Key Features

- **Algorithmic Randomization:** Uses time-seeded pseudo-random number generation to ensure unpredictable computer moves.
- **Robust Input Validation:** Standardized input parsing that handles invalid numbers, characters, and spaces smoothly.
- **Optimized Performance:** Minimal memory footprint with efficient structural layout and instant execution.

---

## Tech Stack

- **Language:** Pure C (C99 Standard or higher)
- **Standard Libraries:** `stdio.h` (I/O operations), `stdlib.h` (randomization/utilities), `time.h` (seeding engine)
- **Compiler Support:** GCC, Clang, MSVC

---

## Getting Started

Follow these instructions to compile and play the game locally on your machine.

### Prerequisites

Ensure you have a C compiler installed on your system:
- **Linux/macOS:** GCC or Clang (usually pre-installed or available via `build-essential` / `Xcode Command Line Tools`)
- **Windows:** MinGW, MSVC (via Visual Studio), or WSL

Check your compiler version by running:
```bash
gcc --version
```

### Compilation

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
   gcc rock_paper-scissors.c -o rps_game
   ```

### Running the Game

Execute the compiled binary from your terminal:

- **Linux/macOS:**
  ```bash
  ./rps_game
  ```
- **Windows:**
  ```cmd
  rps_game.exe
  ```

---

## How to Play

1. Upon launch, the terminal will display a selection menu.
2. Enter your choice corresponding to the menu item:
   - `0` for **Rock**
   - `1` for **Paper**
   - `2` for **Scissors**
3. The computer will immediately reveal its randomized choice.
4. The system evaluates the winner based on classic rules:
   - Rock beats Scissors
   - Scissors beats Paper
   - Paper beats Rock
---

## Project Structure

```text
rock-paper-scissors-c/
├── rock_paper_scissors.c           # Main game loop, input handling, and core logic
├── README.md        # Project documentation

```
