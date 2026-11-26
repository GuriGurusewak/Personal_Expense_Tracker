#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPENSES 100
#define FILE_NAME "expenses.dat"

/* Structure to store one expense */
struct Expense {
    int id;
    char date[15];
    char category[20];
    char note[50];
    float amount;
};

/* Global variables (simple and easy to manage) */
struct Expense expenses[MAX_EXPENSES];
int count = 0;
int nextId = 1;
float budget = 5000.0;

/* Helper function to clear the terminal screen */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Helper function to clear leftover characters in input buffer */
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Helper function to pause until user presses Enter */
void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

/* Save all data to a binary file */
void saveData() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("\nError: Could not save data to file.\n");
        return;
    }

    fwrite(&budget, sizeof(float), 1, fp);
    fwrite(&nextId, sizeof(int), 1, fp);
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(expenses, sizeof(struct Expense), count, fp);

    fclose(fp);
}

/* Load previously saved data from file */
void loadData() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        return; /* File does not exist yet (first time running) */
    }

    fread(&budget, sizeof(float), 1, fp);
    fread(&nextId, sizeof(int), 1, fp);
    fread(&count, sizeof(int), 1, fp);
    fread(expenses, sizeof(struct Expense), count, fp);

    fclose(fp);
}

/* Calculate total amount spent so far */
float getTotalSpent() {
    float total = 0.0;
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
    }
    return total;
}

/* Print a clean top header box */
void printHeader() {
    float total = getTotalSpent();
    float remaining = budget - total;

    printf("===========================================================================\n");
    printf("                        PERSONAL EXPENSE TRACKER                           \n");
    printf("===========================================================================\n");
    printf(" Budget: Rs. %-10.2f | Spent: Rs. %-10.2f | ", budget, total);
    if (remaining >= 0) {
        printf("Left: Rs. %.2f\n", remaining);
    } else {
        printf("Over Budget: Rs. %.2f!\n", -remaining);
    }
    printf("===========================================================================\n");
}

/* Print table top border and column names */
void printTableHeader() {
    printf("+----+------------+---------------+------------------------+------------+\n");
    printf("| ID | Date       | Category      | Note                   | Amount(Rs) |\n");
    printf("+----+------------+---------------+------------------------+------------+\n");
}

/* Print one row of the table */
void printTableRow(struct Expense e) {
    printf("| %-2d | %-10.10s | %-13.13s | %-22.22s | %10.2f |\n",
           e.id, e.date, e.category, e.note, e.amount);
}

/* 1. Add a new expense */
void addExpense() {
    clearScreen();
    printHeader();
    printf("\n--- ADD NEW EXPENSE ---\n\n");

    if (count >= MAX_EXPENSES) {
        printf("Expense list is full! Delete some old expenses first.\n");
        pauseScreen();
        return;
    }

    struct Expense e;
    e.id = nextId;

    printf("Enter Date (DD-MM-YYYY) : ");
    scanf("%14s", e.date);
    clearBuffer();

    /* Let user pick a category easily */
    int catChoice;
    printf("\nSelect Category:\n");
    printf("  1. Food\n");
    printf("  2. Travel\n");
    printf("  3. Study / Books\n");
    printf("  4. Bills / Recharge\n");
    printf("  5. Shopping\n");
    printf("  6. Other\n");
    printf("Enter choice (1-6)      : ");
    if (scanf("%d", &catChoice) != 1) {
        catChoice = 6;
    }
    clearBuffer();

    switch (catChoice) {
        case 1: strcpy(e.category, "Food"); break;
        case 2: strcpy(e.category, "Travel"); break;
        case 3: strcpy(e.category, "Study"); break;
        case 4: strcpy(e.category, "Bills"); break;
        case 5: strcpy(e.category, "Shopping"); break;
        default: strcpy(e.category, "Other"); break;
    }

    printf("Enter Short Note        : ");
    scanf(" %49[^\n]", e.note);
    clearBuffer();

    printf("Enter Amount (Rs.)      : ");
    while (scanf("%f", &e.amount) != 1 || e.amount <= 0) {
        clearBuffer();
        printf("Invalid amount! Enter a positive number: ");
    }
    clearBuffer();

    expenses[count] = e;
    count++;
    nextId++;
    saveData();

    printf("\nExpense added successfully! (ID: %d)\n", e.id);
    pauseScreen();
}

/* 2. View all recorded expenses */
void viewExpenses() {
    clearScreen();
    printHeader();
    printf("\n--- ALL EXPENSES ---\n\n");

    if (count == 0) {
        printf("No expenses recorded yet.\n");
        pauseScreen();
        return;
    }

    printTableHeader();
    for (int i = 0; i < count; i++) {
        printTableRow(expenses[i]);
    }
    printf("+----+------------+---------------+------------------------+------------+\n");
    printf(" Total Items: %-4d                                   Total: Rs. %8.2f\n",
           count, getTotalSpent());

    pauseScreen();
}

/* 3. Search expenses by category */
void searchByCategory() {
    clearScreen();
    printHeader();
    printf("\n--- SEARCH BY CATEGORY ---\n\n");

    if (count == 0) {
        printf("No expenses to search.\n");
        pauseScreen();
        return;
    }

    int catChoice;
    char target[20];

    printf("Select Category to View:\n");
    printf("  1. Food\n");
    printf("  2. Travel\n");
    printf("  3. Study\n");
    printf("  4. Bills\n");
    printf("  5. Shopping\n");
    printf("  6. Other\n");
    printf("Enter choice (1-6): ");
    if (scanf("%d", &catChoice) != 1) {
        clearBuffer();
        printf("\nInvalid choice.\n");
        pauseScreen();
        return;
    }
    clearBuffer();

    switch (catChoice) {
        case 1: strcpy(target, "Food"); break;
        case 2: strcpy(target, "Travel"); break;
        case 3: strcpy(target, "Study"); break;
        case 4: strcpy(target, "Bills"); break;
        case 5: strcpy(target, "Shopping"); break;
        case 6: strcpy(target, "Other"); break;
        default:
            printf("\nInvalid choice.\n");
            pauseScreen();
            return;
    }

    printf("\nShowing expenses for category: %s\n\n", target);
    printTableHeader();

    float catTotal = 0.0;
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(expenses[i].category, target) == 0) {
            printTableRow(expenses[i]);
            catTotal += expenses[i].amount;
            found++;
        }
    }

    if (found == 0) {
        printf("|                  No expenses found in this category.                  |\n");
    }
    printf("+----+------------+---------------+------------------------+------------+\n");
    printf(" Found: %-4d                                      Subtotal: Rs. %8.2f\n",
           found, catTotal);

    pauseScreen();
}

/* 4. Show category-wise spending summary */
void viewSummary() {
    clearScreen();
    printHeader();
    printf("\n--- SPENDING SUMMARY ---\n\n");

    if (count == 0) {
        printf("No data available yet.\n");
        pauseScreen();
        return;
    }

    char categories[6][20] = {"Food", "Travel", "Study", "Bills", "Shopping", "Other"};
    float catSum[6] = {0};
    float total = getTotalSpent();
    float highest = expenses[0].amount;
    int highIndex = 0;

    for (int i = 0; i < count; i++) {
        if (expenses[i].amount > highest) {
            highest = expenses[i].amount;
            highIndex = i;
        }
        for (int j = 0; j < 6; j++) {
            if (strcmp(expenses[i].category, categories[j]) == 0) {
                catSum[j] += expenses[i].amount;
            }
        }
    }

    printf("Total Expenses   : %d\n", count);
    printf("Total Spent      : Rs. %.2f\n", total);
    printf("Average Expense  : Rs. %.2f\n", total / count);
    printf("Highest Expense  : Rs. %.2f (%s - %s)\n\n",
           expenses[highIndex].amount,
           expenses[highIndex].category,
           expenses[highIndex].note);

    printf("+---------------+--------------+------------+\n");
    printf("| Category      | Amount (Rs.) | Percentage |\n");
    printf("+---------------+--------------+------------+\n");
    for (int j = 0; j < 6; j++) {
        float percent = (catSum[j] / total) * 100.0;
        printf("| %-13s | %12.2f | %8.1f %% |\n", categories[j], catSum[j], percent);
    }
    printf("+---------------+--------------+------------+\n");

    pauseScreen();
}

/* 5. Delete an expense by ID */
void deleteExpense() {
    clearScreen();
    printHeader();
    printf("\n--- DELETE EXPENSE ---\n\n");

    if (count == 0) {
        printf("No expenses to delete.\n");
        pauseScreen();
        return;
    }

    printTableHeader();
    for (int i = 0; i < count; i++) {
        printTableRow(expenses[i]);
    }
    printf("+----+------------+---------------+------------------------+------------+\n");

    int delId;
    printf("\nEnter ID of expense to delete (0 to cancel): ");
    if (scanf("%d", &delId) != 1) {
        clearBuffer();
        printf("Invalid input.\n");
        pauseScreen();
        return;
    }
    clearBuffer();

    if (delId == 0) return;

    int pos = -1;
    for (int i = 0; i < count; i++) {
        if (expenses[i].id == delId) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nExpense with ID %d not found.\n", delId);
    } else {
        /* Shift remaining elements to the left */
        for (int i = pos; i < count - 1; i++) {
            expenses[i] = expenses[i + 1];
        }
        count--;
        saveData();
        printf("\nExpense ID %d deleted successfully.\n", delId);
    }

    pauseScreen();
}

/* 6. Update monthly budget */
void setBudget() {
    clearScreen();
    printHeader();
    printf("\n--- SET MONTHLY BUDGET ---\n\n");
    printf("Current Budget: Rs. %.2f\n", budget);
    printf("Enter New Budget (Rs.): ");

    float newBudget;
    if (scanf("%f", &newBudget) == 1 && newBudget > 0) {
        budget = newBudget;
        saveData();
        printf("\nBudget updated to Rs. %.2f successfully!\n", budget);
    } else {
        printf("\nInvalid amount! Budget was not changed.\n");
    }
    clearBuffer();
    pauseScreen();
}

/* Main function with menu loop */
int main() {
    int choice;

    loadData(); /* Load saved data when program starts */

    do {
        clearScreen();
        printHeader();

        printf("\n");
        printf("   +----------------------------------+\n");
        printf("   |            MAIN MENU             |\n");
        printf("   +----------------------------------+\n");
        printf("   |  1. Add New Expense              |\n");
        printf("   |  2. View All Expenses            |\n");
        printf("   |  3. Search by Category           |\n");
        printf("   |  4. View Spending Summary        |\n");
        printf("   |  5. Delete an Expense            |\n");
        printf("   |  6. Set Monthly Budget           |\n");
        printf("   |  0. Exit                         |\n");
        printf("   +----------------------------------+\n");
        printf("\n   Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = -1;
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: searchByCategory(); break;
            case 4: viewSummary(); break;
            case 5: deleteExpense(); break;
            case 6: setBudget(); break;
            case 0:
                printf("\nThank you for using Personal Expense Tracker. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please select between 0 and 6.\n");
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
