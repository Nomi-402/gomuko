# 🎮 Gomoku — Console-Based C++ Game

> **First Project | 1st Semester | BS Artificial Intelligence**  
> Built entirely from scratch in **C++ (Console Application)** using Windows API.  
> Student ID: `BSAI25033`

---

## 📌 What is Gomoku?

**Gomoku** (also known as *Five in a Row*) is a classic two-player strategy board game.  
The objective is simple: be the **first player to get N of your symbols in a row** — horizontally, vertically, or diagonally.

This version puts you (Human) against a **smart Computer AI** on a fully customizable board, all rendered live in the Windows console.

---

## 🖥️ Game Preview

```
------------------
---X--------------
---XO-------------
---XO-------------
---XO-------------   ← Computer blocking Human's column!
------------------

Human (X) Turn:
```

```
╔══════════════════════════╗
║   Human (X) is the Winner! ║
╚══════════════════════════╝
```

---

## ✨ Features

| Feature | Details |
|---|---|
| 🧠 AI Opponent | Win → Block → Center → First Empty strategy |
| 🎯 Custom Board | You choose rows, columns, and win condition at start |
| 🖱️ Mouse Input | Click directly on the board to place your symbol |
| 🎨 Colored Output | X = Red, O = Cyan, Empty = Gray |
| 🏆 Win Detection | All 8 directions: horizontal, vertical, diagonal |
| 🤝 Draw Detection | Detected when board is completely full |
| 🎲 Random First Turn | Randomly decides who goes first each game |

---

## 🚀 How to Run

### Requirements
- **OS:** Windows (uses `windows.h`, `conio.h`)
- **Compiler:** MinGW (g++) or MSVC (Visual Studio)
- **Standard:** C++11 or later

### Steps

**Option 1 — Visual Studio:**
1. Create a new **Console Application** project
2. Replace the default `.cpp` file content with the game code
3. Press `F5` (Run)

**Option 2 — g++ (MinGW Command Line):**
```bash
g++ BSAI25033-PROJECT-2-PHASE-2-FIXED.cpp -o gomoku.exe
gomoku.exe
```

> ⚠️ **Must be compiled for Windows only.** Does not support Linux/macOS due to Windows API dependency.

---

## 🕹️ How to Play

1. **Launch** the program
2. **Enter** your game settings when prompted:
   ```
   Enter WIN_COUNT ROWS COLUMNS: 5 10 15
   ```
   - `WIN_COUNT` = how many in a row to win (e.g., 5 for classic Gomoku)
   - `ROWS` = number of board rows (max 100)
   - `COLUMNS` = number of board columns (max 100)

3. The board appears. **Click on any `-` cell** to place your symbol (`X`)
4. The **Computer (`O`)** responds automatically after a short pause
5. First to get `WIN_COUNT` in a row **wins!**

---

## 🤖 AI Logic (Computer Player)

The Computer opponent uses a **priority-based greedy strategy:**

```
Priority 1 ── Can I WIN right now?        → Play winning move
Priority 2 ── Can Human WIN next turn?    → Block that move
Priority 3 ── Is the center cell free?    → Take the center
Priority 4 ── Fallback                    → Take any empty cell
```

> This is a **Phase 2** implementation. The AI does not use Minimax yet, but it successfully defends and attacks in most scenarios.

---

## 📂 Project Structure

```
📁 Project
├── BSAI25033-PROJECT-2-PHASE-2-FIXED.cpp   ← Main source file (bug-fixed)
└── README.md                                ← This file
```

---

## 🐛 Logical Bugs Fixed (vs Original Phase 2 Submission)

| # | Function | Bug | Fix Applied |
|---|---|---|---|
| 1 | `h_left()` | `c - WIN_COUNT < 0` is off-by-one — rejects valid moves | Changed to `c - (WIN_COUNT - 1) < 0` |
| 2 | `v_up()` | `r - WIN_COUNT < 0` same off-by-one issue | Changed to `r - (WIN_COUNT - 1) < 0` |
| 3 | `d_rdown()` | `c - WIN_COUNT < -1` wrong comparison | Changed to `c - (WIN_COUNT - 1) < 0` |
| 4 | `d_rup()` | `c - WIN_COUNT < -1` wrong comparison | Changed to `c - (WIN_COUNT - 1) < 0` |
| 5 | `d_lup()` | `c + WIN_COUNT > C` allows out-of-bounds access | Changed to `c + (WIN_COUNT - 1) >= C` |
| 6 | Draw Box | `gotoRowCol(10, 8)` placed text outside the drawn box | Corrected to `gotoRowCol(6, 10)` to center inside box |
| 7 | AI `humanTurn` | Hardcoded `humanTurn = 0` breaks if turn order changes | Now computed dynamically as `(turn + 1) % NOP` |
| 8 | Mouse Event | No `EventType == MOUSE_EVENT` check — keyboard events could trigger false clicks | Added `InputRecord.EventType == MOUSE_EVENT` guard |
| 9 | Win checks | Used `count == WIN_COUNT` counter (misses gaps) | Replaced with direct `!= P_sym` early-exit loop |
| 10 | Board colors | All cells printed in same color | Added per-symbol color: X=Red, O=Cyan, `-`=Dark Gray |

---

## 📐 Core Functions Explained

```cpp
initialization()    // Sets up board, player names, symbols, random first turn
display_board()     // Renders the board with colors to console
update_board()      // Places a symbol at given row/col
winning()           // Checks all 8 directions from last move for a win
is_draw()           // Returns true if no empty cell remains
computer_move()     // AI: Win > Block > Center > Fallback
turn_change()       // Alternates turn between 0 and 1
getRowColbyLeftClick() // Captures mouse left-click position via Windows API
print_box()         // Draws a solid-border box for win/draw announcements
```

---

## 📊 Win Condition Directions Checked

```
  ↖  ↑  ↗
  ←  ●  →
  ↙  ↓  ↘
```

All **8 directions** are checked from the last placed piece, making detection efficient — only the affected position is evaluated each turn.

---

## 💡 Concepts Practiced

- 2D Arrays for board state management
- Functions with pass-by-reference parameters
- Windows Console API (`SetConsoleCursorPosition`, `SetConsoleTextAttribute`, `ReadConsoleInput`)
- Mouse input handling in console
- Basic AI greedy strategy
- Boundary checking and edge case handling
- Game loop design pattern

---

## 👤 Author

| Field | Info |
|---|---|
| **Name** | BSAI25033 |
| **Program** | BS Artificial Intelligence |
| **Semester** | 1st Semester |
| **Project** | Project 2 — Phase 2 |
| **Language** | C++ (Console, Windows) |

---

## 📝 License

This project was created as an academic submission. Feel free to use it for learning purposes.

---

*Made with ❤️ and way too many `for` loops — first semester, first project, all from scratch.*
