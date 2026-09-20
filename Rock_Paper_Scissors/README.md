# Rock, Paper, Scissors (CLI)

> A lightweight, highly responsive Command Line Interface (CLI) implementation of the classic Rock, Paper, Scissors game written in pure C.

![Gameplay Preview](https://github.com/sergio-a-juarez-1/C-Projects/blob/main/Rock_Paper_Scissors/gameplay_assets/game.gif?raw=true)

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

## 🎮 How to Play

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

## 🛠️ Build and Local Deployment

To compile and run this program, you will need a standard C compiler (GCC, Clang, or MinGW) installed on your system.

### 1. Isolate the Project via Sparse-Checkout
To deploy this specific game without pulling down your entire monorepo, initialize an empty directory and pull down only the target folder:

```bash
# Initialize an empty local repository
mkdir Rock_Paper_Scissors && cd Rock_Paper_Scissors
git init

# Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and target the specific application folder
git sparse-checkout set Rock_Paper_Scissors

# Pull down only that folder's files
git pull origin main
```

### 2. Compilation
Compile the source file using GCC:
```bash
gcc rock_paper_scissors.c -o rps_game
```

### 3. Running the Game
Execute the compiled binary from your terminal:

* **Linux/macOS:**
  ```bash
  ./rps_game
  ```
* **Windows:**
  ```cmd
  rps_game.exe
  ```

---

## 📂 Project Structure

```text
Rock_Paper_Scissors/
├── rock_paper_scissors.c    # Main game loop, input handling, and core logic
└── README.md                # Project documentation
```
