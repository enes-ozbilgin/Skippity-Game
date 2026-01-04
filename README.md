# Skippity Board Game in C

A C implementation of the strategy board game **Skippity**. This project runs in the command line and simulates the mechanics of jumping and capturing pieces to collect different colors.

## 🧩 Overview

Skippity is a game of jumping and capturing. The goal is to collect sets of different colored pieces (skippers) by jumping over them on the board.

**Key Features:**
* **Full Game Logic:** Validates legal moves, jumps, and captures.
* **Save & Load System:** The game state can be saved to `savedgame.txt` and resumed later.
* **Console Interface:** Visualizes the board using text/ASCII characters.

## 📂 Repository Contents

* `Skippity_Game.c`: The main source code for the game.
* `savedgame.txt`: Stores the data for the saved game state.
* `Skippity_Game.exe`: Pre-compiled executable (Windows).

## 🚀 How to Compile and Run

While a pre-compiled `.exe` is included, it is recommended to compile the source code yourself to ensure compatibility with your system.

### 1. Compile
Open your terminal or command prompt in the project folder and run:

```bash
gcc Skippity_Game.c -o skippity
