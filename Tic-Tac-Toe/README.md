# Tic-Tac-Toe Game in C

A lightweight, terminal-based **Tic-Tac-Toe** game written in C. This project features a 2-player local multiplayer experience with dynamic board rendering, input validation, and automatic win/draw detection.

---

## Features

* **Local 2-Player Gameplay:** Alternates turns dynamically between Player 1 (X) and Player 2 (O).
* **Dynamic Board Layout:** Refreshes the terminal display after every move to show real-time game status.
* **Input Validation:** Prevents players from choosing an already-occupied cell or invalid entry, keeping the turn active.
* **Automatic Game Resolution:** Continuously evaluates the board state to instantly declare a win, a loss, or a draw.

---

## 🎮 How to Play

1. The game is played on a 3x3 grid numbered from 1 to 9.
2. Player 1 is assigned **X** and Player 2 is assigned **O**.
3. On your turn, type the number corresponding to the cell where you want to place your mark, then press **Enter**.
4. The first player to align 3 of their marks horizontally, vertically, or diagonally wins.
5. If all 9 squares are filled without a winner, the game results in a draw.

---

## 🛠️ Build and Local Deployment

To compile and run this program, you will need a standard C compiler (GCC, Clang, or MinGW) installed on your system.

### 1. Isolate the Project via Sparse-Checkout
To deploy this specific game without pulling down your entire monorepo, initialize an empty directory and pull down only the target folder:

```bash
# Initialize an empty local repository
mkdir Tic-Tac-Toe && cd Tic-Tac-Toe
git init

# Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and target the specific application folder
git sparse-checkout set Tic-Tac-Toe

# Pull down only that folder's files
git pull origin main
```

### 2. Compilation
Compile the program using GCC:
```bash
gcc tic_tac_toe.c -o tictactoe
```

### 3. Running the Application
Run the compiled executable from your terminal:

* **Linux / macOS:**
  ```bash
  ./tictactoe
  ```
* **Windows:**
  ```cmd
  tictactoe.exe
  ```

---

## 📂 Code Architecture

The game is structured using three core procedural functions:
* `displayBoard()`: Renders the 3x3 layout and prints the player labels. Clears the screen using system utilities before drawing.
* `markBoard(char mark)`: Validates user placement options and safe-guards against illegal cell overwrites.
* `checkForWin()`: Inspects all 8 possible winning vectors alongside board saturation states to return programmatic flags (`1` for win, `0` for draw, `-1` for ongoing).
