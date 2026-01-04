# Burger QuizBot

This project is due on the 18th of December and is worth 40% of our final grade.

Objective: Build an interactive console-based quiz chat-bot in C language that allows users to answer questions, track their score, and save their progress. All user data, history, and quiz questions are stored in files managed by the system.

---

## Key Features

### Player Mode
* **Dynamic Quiz:** Sessions of 20 random questions drawn from an extensible database.
* **Difficulty Levels:** Filter questions by level (Easy, Medium, Expert, or Mixed).
* **Fuzzy Logic Engine:** Intelligent answer recognition that tolerates typos, missing accents, and case sensitivity differences.
* **Progression Tracking:** Complete history of games played, score calculation (/20), and High Score persistence.
* **Leaderboard:** Display of the Top 10 best players.

### Administrator Mode
* **User Management:** Ability to delete user accounts.
* **Content Management:** Dynamic addition of new questions without needing to recompile the source code.
* **Security:** The first user created upon initialization is automatically granted Administrator privileges.

---

## Technical Architecture

The project follows a strict modular architecture separating interface, implementation, and data.

### Source Files
* **`quizbot.h` (Interface):** Defines data structures (`User`, `Question`, `HistoryEntry`), constants, and function prototypes.
* **`quizbot.c` (Implementation):** Contains business logic, file handling subsystems, and text processing algorithms.

### Data Persistence
The program utilizes a Hybrid Text/Binary system to combine ease of editing with runtime performance:

1.  **`init_questions.txt` (Source):** Human-readable text file (CSV format) used for initial import or database restoration.
2.  **`questions.dat` (Binary):** Optimized database generated automatically upon first launch.
3.  **`utilisateurs.dat` (Binary):** Storage for user profiles and access rights.
4.  **`historique.dat` (Binary):** Logging of scores, timestamps, and game metadata.

> **Technical Note:** All file write operations use an **atomic method** (write to `.tmp` $\to$ backup to `.bak` $\to$ rename) to guarantee data integrity even in the event of a crash.

---

##  Installation & Compilation

### Prerequisites
* A standard C compiler.

### Compilation
Open a terminal in the project directory and run:

```bash
gcc -std=c11 -Wall -Wextra -o quizbot quizbot.c
```

### Launch
Then launch the game by typing:

```bash
./quizbot