![NATO GUI](https://github.com/sergio-a-juarez-1/C-practice/blob/main/Images/nato_alphabet.png?raw=true)

# NATO Phonetic Alphabet GUI Dashboard

A high-performance graphical desk utility written in C that parses a CSV dataset to convert typed input text into its corresponding NATO phonetic alphabet code words. This project swaps high-level Python Pandas and Tkinter abstractions for an ultra-fast, immediate-mode GUI environment powered by Raylib, native file I/O operations, and optimized array-offset memory mapping.

---

## Key Features

- **Live Alphanumeric Translation:** Utilizes an immediate-mode UI rendering layout to process text and update phonetic sequences in real-time as keys are hit.
- **Pure C Text Wrapping Engine:** Implements a custom structural character-length wrapping subroutine (`DrawTextWrapped`) to cleanly align text tokens without relying on external system engines.
- **Immersive OLED Dark Theme:** Engineered with a deep charcoal background palette and bright electric neon accent interfaces to optimize look, readability, and eye strain.
- **Robust Space Parsing:** Automatically intercepts blank character boundaries to track multi-word sentence structures clearly as visual space blocks (`[SPACE]`).
- **Standard Build Automation:** Integrated with an efficient execution pipeline managed by a centralized Makefile to optimize workspace deployments.

---

## 🎮 How It Works & Example Run

The application automatically reads the local `alphabet.csv` matrix database upon initialization to process incoming window events interactively:

```text
Enter a word or phrase:
[ taxi cab             ] [ CLEAR ]

Result:
Tango Alfa X-ray India [SPACE] Charlie Alfa Bravo
```

---

## 🛠️ Build and Local Deployment

To compile and run this application, you will need a standard C compiler (GCC/Clang), GNU Make, and Raylib dependency drivers installed on your system framework.

### 1. Isolate the Project via Sparse-Checkout
To deploy this specific utility without pulling down your entire monorepo, initialize an empty directory and target only this branch folder:

```bash
# Initialize an empty local repository
mkdir Nato_Alphabet && cd Nato_Alphabet
git init

# Add your multi-project repo as the remote origin
git remote add origin https://github.com/sergio-a-juarez-1/C-Projects.git

# Enable sparse-checkout and target the specific application folder
git sparse-checkout set Nato_Alphabet

# Pull down only that folder's files
git pull origin main
```

### 2. Compilation via Build Automation
Compile the graphics engine using your predefined Makefile:
```bash
make
```
*(To flush the binaries out and reset your canvas workspace, you can execute `make clean` anytime).*

### 3. Running the Application
Ensure `alphabet.csv` and `raygui.h` are located in your executing directory alongside your binary, then type:

```bash
./gui_nato
```

---

## 📂 Deep Dive Under the Hood

The execution sequence manages graphical frame buffers and lookup vectors directly in raw memory:
1. **Low-Level Token Serialization:** Parses incoming structural tables by skipping descriptive layout definitions through string checks, populating character positions directly down into a multidimensional memory grid (`char nato_dict[26][50]`).
2. **Immediate Mode UI Execution Loop:** Synchronizes character capture cycles (`GetCharPressed()`) with real-time text arrays, intercepting key modification states dynamically before window buffers draw.
3. **Array Layout Protection:** Implements protective structural clearing routines (`memset()`) to erase string traces upon activation of the reset controls, avoiding memory bleed or garbage text leakage.
