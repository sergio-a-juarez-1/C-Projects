# Tic-Tac-Toe Game in C

A lightweight, terminal-based **Tic-Tac-Toe** game written in C. This project features a 2-player local multiplayer experience with dynamic board rendering, input validation, and automatic win/draw detection.

---

## Features

* **Local 2-Player Gameplay:** Alternates turns dynamically between Player 1 (X) and Player 2 (O).
* **Dynamic Board Layout:** Refreshes the terminal display after every move to show real-time game status.
* **Input Validation:** Prevents players from choosing an already-occupied cell or invalid entry, keeping the turn active.
* **Automatic Game Resolution:** Continuously evaluates the board state to instantly declare a win, a loss, or a draw.

## How to Play

1. The game is played on a 3x3 grid numbered from 1 to 9.
2. Player 1 is assigned **X** and Player 2 is assigned **O**.
3. On your turn, type the number corresponding to the cell where you want to place your mark, then press **Enter**.
4. The first player to align 3 of their marks horizontally, vertically, or diagonally wins.
5. If all 9 squares are filled without a winner, the game results in a draw.

## Prerequisites

To compile and run this program, you will need a C compiler installed on your system:
* **GCC / Clang:** Standard Linux/macOS compilers.
* **MinGW / MSVC:** Windows compilers.

## Compilation and Installation

1. Clone or download the repository containing the source code.
2. Open your terminal or command prompt and navigate to the folder:
   ```bash
   cd path/to/Tic-Tac-Toe
   ```
3. Compile the program using GCC:
   ```bash
   gcc tic_tac_toe.c -o tictactoe
   ```

## Usage

Run the compiled executable from your terminal:

* **Windows:**
  ```cmd
  tictactoe.exe
  ```
* **Linux / macOS:**
  ```bash
  ./tictactoe
  ```

## Code Architecture

The game is structured using three core procedural functions:
* `displayBoard()`: Renders the 3x3 layout and prints the player labels. Clears the screen using system utilities before drawing.
* `markBoard(char mark)`: Validates user placement options and safe-guards against illegal cell overwrites.
* `checkForWin()`: Inspects all 8 possible winning vectors alongside board saturation states to return programmatic flags (`1` for win, `0` for draw, `-1` for ongoing).
