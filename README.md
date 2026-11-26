# Personal Expense Tracker (C)

A simple, terminal-based 
**Personal Expense Tracker** 
written in a single C file. Designed as a first-year B.Tech mini hackathon project using core C programming concepts — no external libraries required.

---

## Features

- **Live Budget Dashboard:** Displays monthly budget, total spent, and remaining (or over-budget) balance at the top of the screen.
- **Add New Expense:** Record expenses with date, predefined category (`Food`, `Travel`, `Study`, `Bills`, `Shopping`, `Other`), short note, and validated amount.
- **View All Expenses:** Displays all records in a neatly aligned ASCII table with item count and total.
- **Search by Category:** Filter expenses by category along with the category subtotal.
- **Spending Summary:** Shows total spent, average expense, highest expense, and a category-wise percentage breakdown.
- **Delete Expense:** Remove any expense by its ID.
- **Set Monthly Budget:** Update your budget limit anytime.
- **Auto-Save Persistence:** Automatically saves and loads data using a binary file (`expenses.dat`).

---

## Concepts Used

- Structures (`struct Expense`) & Arrays of Structures
- Modular Functions & Control Flow (`do-while`, `switch-case`, `for`, `while`)
- Binary File Handling (`fopen`, `fwrite`, `fread`, `fclose`)
- String Handling (`strcpy`, `strcmp`)
- Input Validation & Buffer Clearing (`clearBuffer()`)

---

## How to Compile & Run

### Prerequisites
- A C compiler such as **GCC** (MinGW on Windows, GCC/Clang on Linux/macOS).

### Compile
```bash
gcc expense_tracker.c -o expense_tracker
