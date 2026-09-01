# 🏃‍♂️ Workouts Heart Rate Monitor

A command-line terminal application written in C that calculates a user's target heart rate zones during training based on their age using the authoritative Tanaka formula. It allows athletes to track their intensity levels dynamically in real-time during workouts.

---

## 📊 Heart Rate Zones Architecture

Maximum Heart Rate (\(HR_{max}\)) is calculated utilizing the **Tanaka Formula**:
\[HR_{max} = 208 - (0.7 \times \text{Age})\]

The software monitors and segments performance across six intensity thresholds:

| Zone ID | Intensity Range | Training Phase Description |
| :---: | :---: | :--- |
| **1** | Below 50% | **Rest** |
| **2** | 50% – 60% | **Very light**: Active recovery |
| **3** | 60% – 70% | **Light**: Cardio training, improves general endurance |
| **4** | 70% – 80% | **Moderate**: Aerobic fitness, improves efficiency |
| **5** | 80% – 90% | **Hard**: Raises anaerobic threshold, improves speed endurance |
| **6** | Above 90% | **Peak performance**: Extreme power development (Pros only) |

---

## 🛠️ Compilation and Installation

### Prerequisites
Ensure you have a standard C compiler such as `gcc` or `clang` installed on your host system.

### Compilation
Compile the standard source code file using the following terminal command:
```bash
gcc -O2 main.c -o hr_monitor
```

---

## 💻 Usage

Execute the compiled application binary:
```bash
./hr_monitor
```

### Application Lifecycle Flow
1. **Demographics Input**: The program prompts for your current age.
2. **Zone Mapping**: It outputs your absolute \(HR_{max}\) alongside a structured tabular summary mapping out your personalized BPM boundaries.
3. **Real-time Loop Monitoring**: Enter any arbitrary numeric BPM value to check your current training zone footprint.
4. **Termination Sequence**: Input `0` or any negative value (`-1`) to safely terminate execution loop instances.

### Execution Example
```text
=== Workouts Heart Rate Monitor ===

Enter your age: 25
Your maximum heart rate is 190

1 |   0 -  95 | rest
2 |  95 - 114 | very light, active recovery
3 | 114 - 133 | light, cardio training, improves your general endurance
4 | 133 - 152 | moderate, aerobic fitness, improve your efficiency
5 | 152 - 171 | hard, raise the anaerobic threshold, improve your speed endurance
6 | 171 - 190 | peak performance, improve your power, only for pros!

Enter the current BPM: 145
Zone 4: moderate, aerobic fitness, improve your efficiency

Enter the current BPM: 0
```
