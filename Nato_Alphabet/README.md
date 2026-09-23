# C-Password Generator GUI Utility

A low-latency, zero-dependency desktop utility written in pure C that builds, randomizes, shuffles, and profiles high-entropy security credentials. This project swaps heavy third-party layout engines for a custom, ultra-fast immediate-mode graphical environment operating directly over the native Linux X11/Xlib protocol structures.

---

## Key Features

- **Live Alphanumeric Parameter Structuring:** Utilizes interactive increment buttons to modify character array distributions, recalculating password composition layout pools on the fly.
- **Real-Time Entropy Profiling:** Implements an integrated security metric subroutine that dynamically audits character balance ratios to classify credentials as WEAK, MEDIUM, or STRONG.
- **Native Selection Handshake:** Implements a direct X11 Clipboard inter-process communication handler (`CLIPBOARD`/`UTF8_STRING` Atoms) to instantly staging keys into the OS cache upon clicking the container card.
- **OLED Matte Dark Theme:** Configured with high-contrast deep charcoal backdrops and vibrant neon-green layout boundaries to guarantee excellent legibility under all desktop environments.
- **Zero Library Bloat:** Operates with absolute freedom from external engines, matching strict memory management routines to keep execution footprints minimal.

---

## 🎮 How It Works & Example Run

The application initializes an independent window thread and polls local desktop inputs to synchronize password buffers instantaneously:

```text
PASSWORD GENERATOR
Configure text arrays to sample output layout pools:

  Letters Count:    [ - ]  14  [ + ]
  Symbols Count:    [ - ]   4  [ + ]
  Numbers Count:    [ - ]   7  [ + ]

  Password Strength: STRONG
  
  Generated Key Layout (Click box to Copy):
  [ us@1w4!7x1*g$I6kCSOY2o2Hx                     ]
  
  Success: Password copied to OS clipboard tracker!
```

---

## 🛠️ Build and Local Deployment

To compile and run this application, you will need a standard C compiler (GCC/Clang), GNU Make, and base X11 core developer libraries installed on your distribution pipeline.

### 1. Isolate the Project via Sparse-Checkout
To deploy this specific utility without pulling down your entire monorepo, initialize an empty directory and target only this branch folder:

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

### 2. Compilation via Build Automation
Compile the desktop window system using your predefined automation script:
```bash
make
```
*(To flush out previous binaries and reset your local workspace folder, you can run `make clean` at any time).*

### 3. Running the Application
Launch the standalone binary directly from your active desktop terminal frame:

```bash
./gui_password
```

---

## 📂 Deep Dive Under the Hood

The execution loop processes graphical state changes and memory layouts with low-overhead efficiency:
1. **Fisher-Yates Data Shuffling:** Processes randomized pools consecutively across local loops before applying an in-place Fisher-Yates array-offset swapping matrix to prevent systemic predictability patterns.
2. **Immediate-Mode Graphics Pipelines:** Eliminates retaining scene trees by forcing layout dimensions and input checks (`Expose` and `ButtonPress` events) to cycle synchronously inside a raw `XNextEvent` handler loop.
3. **Protected Inter-Process Communication:** Overrides native clipboard buffers by answering external `SelectionRequest` hooks from other apps via atom handshakes, exposing strings only upon validation.
