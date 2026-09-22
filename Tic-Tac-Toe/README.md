### Tic-Tac-Toe Game in C

A lightweight, terminal-based **Tic-Tac-Toe** game written in C. This enhanced project features local 2-player multiplayer, a smart AI opponent with multiple difficulty levels, cross-round score tracking, and an alternating tournament-style turn structure. 

### ✨ Features

* **Multiple Game Modes:** Play a classic local 2-player game or challenge a simulated computer opponent.
* **Smart AI Opponent:** Features three distinct difficulty settings: 

  * *Easy:* The computer plays completely at random.
  * *Medium:* The computer splits its choices between strategic moves and random actions.
  * *Hard:* Uses the **Minimax Algorithm** to calculate every possible game tree, making the computer completely unbeatable.
* **Persistent Scoreboard:** Tracks Player 1 wins, Player 2/Computer wins, and draws dynamically in the header across matches.
* **Alternating First Turns:** Implements tournament-style play where the starting player alternates automatically with each consecutive rematch.
* **Input Buffer Validation:** Safely clears input streams to prevent input desynchronization, accidental multi-digit entry bugs, or illegal cell overwrites.

### 🎮 How to Play

1. **Game Setup:** Choose your game mode (1 or 2) and select your AI difficulty if playing against the computer.
2. The game is played on a 3x3 grid numbered sequentially from 1 to 9.
3. Player 1 is assigned **X** and Player 2/Computer is assigned **O**.
4. On your turn, type the number (1-9) corresponding to the cell where you want to place your mark, then press **Enter**.
5. The first player to align 3 of their marks horizontally, vertically, or diagonally wins.
6. If all 9 squares are filled without a winner, the game results in a draw, the scores update, and you will be asked if you want to play a rematch.

### 🛠️ Build and Local Deployment

To compile and run this program, you will need a standard C compiler (GCC, Clang, or MinGW) installed on your system. 

# Initialize an empty local repository
mkdir Tic-Tac-Toe && cd Tic-Tac-Toe
git init

# Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and target the specific application folder
git sparse-checkout set Tic-Tac-Toe

# Pull down only that folder's files
git pull origin main


### 2. Compilation

Compile the program using GCC: 

bash

gcc tic_tac_toe.c -o tictactoe

Use code with caution.

### 3. Running the Application

Run the compiled executable from your terminal: 

* **Linux / macOS:** 

bash

./tictactoe

Use code with caution.
* **Windows:** 

cmd

tictactoe.exe

Use code with caution.

### 📂 Code Architecture

The game is structured using an optimized 1D array layout and a recursive state evaluation pipeline: 

* displayBoard(): Clears the terminal using cross-platform system utility commands and renders the live 3x3 board along with the active match scoreboard.
* markBoard(char mark): Inspects choices to confirm bounds (1-9) and validates that slots are structurally empty before placing a character token.
* checkForWin(): Iterates across all 8 victory conditions and draw vectors to return game evaluation values (1 for win, 0 for draw, -1 for ongoing).
* get_computer_move(): Branches strategy parameters between get_random_move() and the lookahead simulator based on selected configurations.
* minimax(int depth, int is_max): A recursive game theory artificial intelligence engine that scores and minimizes/maximizes board outcomes to guarantee flawless computer play.
* resetBoard(): Automatically resets the unmapped character state markers ('1' through '9') safely before triggering a round rematch.
