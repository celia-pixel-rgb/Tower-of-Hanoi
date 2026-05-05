# Tower of Hanoi — Three C Implementations

A collection of three C programs that solve the classic **Tower of Hanoi** puzzle, each offering a different level of interaction and visual output — from a plain text solver to a full GTK4 graphical interface.

---

## Table of Contents

- [About the Problem](#about-the-problem)
- [Implementations Overview](#implementations-overview)
- [File Descriptions](#file-descriptions)
  - [1. Towel\_of\_Hanoi\_steps.c](#1-towel_of_hanoi_stepsc)
  - [2. Tower\_of\_Hanoi\_pegs\_output.c](#2-tower_of_hanoi_pegs_outputc)
  - [3. gtk\_TowerofHanoi.c](#3-gtk_towerofhanoic)
- [Requirements & Dependencies](#requirements--dependencies)
- [Compilation & Usage](#compilation--usage)
- [Algorithm Explanation](#algorithm-explanation)
- [Project Structure](#project-structure)

---

## About the Problem

The **Tower of Hanoi** is a classic mathematical puzzle. Given three pegs and a set of disks of different sizes stacked on the first peg (largest at the bottom), the goal is to move all disks to the third peg following these rules:

1. Only **one disk** can be moved at a time.
2. A disk can only be placed on top of a **larger disk** or on an **empty peg**.
3. The minimum number of moves required for `n` disks is **2ⁿ − 1**.

---

## Implementations Overview

| # | File | Output Type | Platform | Dependencies |
|---|------|------------|----------|--------------|
| 1 | `Towel_of_Hanoi_steps.c` | Text — move list | Cross-platform | None (standard C) |
| 2 | `Tower_of_Hanoi_pegs_output.c` | Text — animated ASCII | Windows only | `windows.h` |
| 3 | `gtk_TowerofHanoi.c` | Graphical GUI | Linux / macOS / Windows | GTK4, Cairo |

---

## File Descriptions

### 1. `Towel_of_Hanoi_steps.c`

**The simplest and most portable implementation.**

Solves the puzzle using pure recursion and prints each move as a line in the format `from → to`. No graphics or animation — just the raw sequence of moves needed to solve the puzzle.

**Key features:**
- Standard C only (`stdio.h`) — compiles anywhere
- Clean recursive `hanoi()` function with clear comments
- Prompts the user for the number of disks at runtime

**Sample output (3 disks):**
```
Steps to solve Tower of Hanoi:

1 -> 3
1 -> 2
3 -> 2
1 -> 3
2 -> 1
2 -> 3
1 -> 3
```

---

### 2. `Tower_of_Hanoi_pegs_output.c`

**An animated ASCII-art visualizer running in the Windows terminal.**

Displays the three pegs and disks graphically using characters (`=` and `|`), refreshing the screen after every move with a 0.5-second delay so you can watch the puzzle being solved step by step.

**Key features:**
- Real-time screen refresh using `system("cls")` and `Sleep()` from `windows.h`
- Disk sizes represented visually — larger disks appear wider
- Peg state stored in a 2D array `tower[3][MAX]` with a `top[]` counter per peg
- Supports up to **10 disks** (configurable via `#define MAX`)
- Input validation to reject invalid disk counts

**Sample display (mid-solve):**
```
          |                   |                   |
          |                   |                   |
          |                   |              =|=
          |                   |            ==|==
     ===|===                  |           ===|===
------------------------------------------------------------
      1                 2                 3
```

> ⚠️ **Windows only** — relies on `windows.h` for `Sleep()` and the `cls` command for screen clearing.

---

### 3. `gtk_TowerofHanoi.c`

**A full graphical application built with GTK4 and Cairo.**

Renders the Tower of Hanoi puzzle in a GUI window with colored disks drawn on a canvas. The user selects the number of disks via a spin button and steps through the solution one move at a time by clicking **Next Move**.

**Key features:**
- GTK4 window with a `GtkDrawingArea` rendered using **Cairo 2D graphics**
- Up to **7 disks**, each with a distinct color
- Solution is **pre-computed** at startup using recursion, then replayed step by step
- Move counter displayed as a status label (`Move X / Y`)
- Button disables and shows **"Solved!"** when the puzzle is complete
- Spin button allows resetting with a different disk count at any time

**GUI layout:**

```
┌─────────────────────────────────────┐
│  [Spin: 1–7]   [Next Move]          │
│                                     │
│   ═══  Canvas with pegs & disks ═══ │
│                                     │
│   Status: Move 3 / 15               │
└─────────────────────────────────────┘
```

---

## Requirements & Dependencies

### File 1 — `Towel_of_Hanoi_steps.c`
- Any C compiler (GCC, Clang, MSVC)
- No external libraries

### File 2 — `Tower_of_Hanoi_pegs_output.c`
- **Windows only**
- GCC via MinGW or MSVC
- `windows.h` (included with Windows SDK / MinGW)

### File 3 — `gtk_TowerofHanoi.c`
- GTK4 development libraries
- Cairo (usually bundled with GTK4)
- GCC or Clang

**Installing GTK4:**

```bash
# Ubuntu / Debian
sudo apt install libgtk-4-dev

# Fedora
sudo dnf install gtk4-devel

# macOS (Homebrew)
brew install gtk4

# Windows
# Use MSYS2: pacman -S mingw-w64-x86_64-gtk4
```

---

## Compilation & Usage

### File 1 — Steps output

```bash
gcc Towel_of_Hanoi_steps.c -o hanoi_steps
./hanoi_steps
```

### File 2 — ASCII animation (Windows)

```bash
gcc Tower_of_Hanoi_pegs_output.c -o hanoi_pegs.exe
hanoi_pegs.exe
```

### File 3 — GTK4 GUI

```bash
gcc gtk_TowerofHanoi.c -o hanoi_gtk \
    $(pkg-config --cflags --libs gtk4) -lm
./hanoi_gtk
```

---

## Algorithm Explanation

All three programs use the same classic **recursive algorithm**:

```
hanoi(n, from, to, aux):
    if n == 1:
        move disk from → to
        return
    hanoi(n - 1, from, aux, to)   # Move n-1 disks out of the way
    move disk from → to            # Move the largest disk
    hanoi(n - 1, aux, to, from)   # Stack n-1 disks on top
```

The difference lies in what "move disk" means in each version:

| Implementation | "Move disk" action |
|---|---|
| `Towel_of_Hanoi_steps.c` | `printf("%d -> %d\n", from, to)` |
| `Tower_of_Hanoi_pegs_output.c` | Update array + redraw ASCII canvas |
| `gtk_TowerofHanoi.c` | Store move in a list → replay on button click |

**Time complexity:** O(2ⁿ) moves  
**Space complexity:** O(n) recursive call stack

---

## Project Structure

```
.
├── Tower of Hanoi with GTK/
│   └── gtk_TowerofHanoi.c             # GTK4 graphical interface (Linux/macOS/Windows)
│
└── Tower of Hanoi Without GTK/
    ├── Towel_of_Hanoi_steps.c         # Recursive text output (cross-platform)
    └── Tower_of_Hanoi_pegs_output.c   # Animated ASCII visualizer (Windows)
```
