#include <stdio.h>
#include <stdlib.h>

#include <time.h>



#define BEGINNER_SIZE 10
#define INTERMEDIATE_SIZE 16
#define EXPERT_SIZE 24
#define BEGINNER_MINES 10
#define INTERMEDIATE_MINES 40
#define EXPERT_MINES 200
#define MAX_ENTRIES 10
#define NAME_LENGTH 20

// Define the structure to hold player information
typedef struct {
    char name[NAME_LENGTH];
    double timeTaken;
} PlayerInfo;

// Function prototypes
void updateLeaderboard(PlayerInfo leaderboard[], int *numEntries, const char *name, double timeTaken);
void displayLeaderboard(const PlayerInfo leaderboard[], int numEntries);
void saveLeaderboard(const PlayerInfo leaderboard[], int numEntries);
void loadLeaderboard(PlayerInfo leaderboard[], int *numEntries);

void initializeBoard(char ***board, int size, int mines) {
    // Initialize the seed for random number generation
    srand(time(NULL));

    // Allocate memory for the board and set all cells to '0'
    *board = malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        (*board)[i] = malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) {
            (*board)[i][j] = '0';
        }
    }

    // Randomly place mines
    int minesPlaced = 0;
    while (minesPlaced < mines) {
        int x = rand() % size;
        int y = rand() % size;
        if ((*board)[y][x] == '0') { // Check if the cell is not already a mine
            (*board)[y][x] = 'm';
            minesPlaced++;
        }
    }
}
void printBoard(char **board, char **displayBoard, int size) {
    // Determine the maximum number of digits for row and column indices
    int maxDigits = 1;
    int temp = size - 1;
    while (temp >= 10) {
        temp /= 10;
        maxDigits++;
    }

    // Calculate the maximum width needed for any content in a cell
    int maxCellWidth = maxDigits * 2 + 3; // For "[%0*d:%0*d]" pattern

    // Print the board
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (displayBoard[i][j] == '0') {
                // Cell is unrevealed, print its coordinates
                printf("[%*d:%*d]", maxDigits, i, maxDigits, j);
            } else {
                // Calculate padding for centering
                int padding = (maxCellWidth - 3) / 2;

                // Print content centered within square brackets
                printf("[%*s%*c%*s]", padding, "", 1, displayBoard[i][j], padding, "");
            }
        }
        printf("\n");
    }
}




void freeBoard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}
int countAdjacentMines(char **board, int size, int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < size && j >= 0 && j < size && board[i][j] == 'm') {
                count++;
            }
        }
    }
    return count;
}

void revealCell(char **board, char **displayBoard, int size, int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size || displayBoard[row][col] != '0') {
        // Cell is out of bounds or already revealed
        return;
    }

    int adjacentMines = countAdjacentMines(board, size, row, col);

    if (board[row][col] == 'm') {
        // Mine found - Game over
        printf("Game Over! You hit a mine at %d,%d.\n", row, col);
        displayBoard[row][col] = '*';  // Mark the hit mine
        // TODO: Reveal the whole board and end the game
    } else if (adjacentMines > 0) {
        // No mine, but there are adjacent mines
        displayBoard[row][col] = '0' + adjacentMines;  // Convert int to char and reveal
    } else {
        // No mine and no adjacent mines, reveal all surrounding cells
        displayBoard[row][col] = ' ';  // Mark as empty
        // Recursively reveal all adjacent cells
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                revealCell(board, displayBoard, size, i, j);
            }
        }
    }
}

void toggleFlag(char **displayBoard, int row, int col) {
    if (displayBoard[row][col] == 'F') {
        // Remove the flag if it's already there
        displayBoard[row][col] = '0';
    } else if (displayBoard[row][col] == '0') {
        // Place a flag if the cell is unrevealed
        displayBoard[row][col] = 'F';
    }
}

int checkWin(char **board, char **displayBoard, int size, int mines) {
    int revealedCells = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (displayBoard[i][j] != '0' && displayBoard[i][j] != 'F') {
                revealedCells++;
            }
        }
    }
    return (revealedCells == size * size - mines);
}


int main() {
    int size, mines;
    char **board, **displayBoard;
    char difficulty;
    int row, col;
    int gameOver = 0;
    int hitMine = 0;  // To track if a mine has been hit
    char playerName[10];

    printf("what's your name ");
    scanf("%s",&playerName[0]);


    PlayerInfo leaderboard[MAX_ENTRIES];
    int numEntries = 0;

     loadLeaderboard(leaderboard, &numEntries);


    clock_t startTime, endTime;

    printf("Select difficulty level (B=Beginner, I=Intermediate, E=Expert, C=Custom): ");
    scanf(" %c", &difficulty);

    if (difficulty == 'C' || difficulty == 'c') {
    printf("Enter custom board size: ");
    scanf("%d", &size);
    printf("Enter custom number of mines: ");
    scanf("%d", &mines);
    }else {
    // Set the size and number of mines based on the selected difficulty level
    switch (difficulty) {
        case 'B':
        case 'b':
            size = BEGINNER_SIZE;
            mines = BEGINNER_MINES;
            break;
        case 'I':
        case 'i':
            size = INTERMEDIATE_SIZE;
            mines = INTERMEDIATE_MINES;
            break;
        case 'E':
        case 'e':
            size = EXPERT_SIZE;
            mines = EXPERT_MINES;
            break;
        default:
            printf("Invalid difficulty level.\n");
            return 1;
    }
    }
    // Initialize the board with mines and a display board for the player
    initializeBoard(&board, size, mines);
    initializeBoard(&displayBoard, size, 0); // Initialize with 0 mines for display

    startTime = clock();
    // Game loop
    while (!gameOver) {
        printBoard(board, displayBoard, size); // Print the display board
        printf("Enter row and column to reveal, or 'f' to place/remove a flag (e.g., '3 4' or 'f 3 4'): ");

        char command;
        if (scanf(" %c", &command) && (command == 'f' || command == 'F')) {
            // If the command is to place/remove a flag
            scanf("%d %d", &row, &col);
            toggleFlag(displayBoard, row, col);
        } else {
            // Assume the command is to reveal a cell
            ungetc(command, stdin); // Put the last read character back into the input stream
            if (scanf("%d %d", &row, &col) != 2) {
                // Input was not two integers, clear the buffer and prompt again
                while ((command = getchar()) != '\n' && command != EOF) { } // Clear the input buffer
                printf("Invalid input. Please enter row and column to reveal (e.g., '3 4').\n");
                continue;
            }

            // Validate input
            if (row < 0 || row >= size || col < 0 || col >= size) {
                printf("Invalid coordinates. Try again.\n");
                continue;
            }

            // If the cell is flagged, don't reveal it
            if (displayBoard[row][col] == 'F') {
                printf("Cell is flagged. Remove the flag or choose another cell.\n");
                continue;
            }

            // Reveal the cell
            revealCell(board, displayBoard, size, row, col);

            // Check for game over condition
            if (board[row][col] == 'm') {
                gameOver = 1;
                hitMine = 1; // A mine was hit

                // Reveal all mines
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        if (board[i][j] == 'm') {
                            displayBoard[i][j] = (i == row && j == col) ? '*' : 'm';
                        }
                    }
                }
            } else if (checkWin(board, displayBoard, size, mines)) {
                gameOver = 1;
                // Reveal all mines since the game is won
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        if (board[i][j] == 'm') {
                            displayBoard[i][j] = 'm';
                        }
                    }
                }
            }
        }

        if (gameOver) {
             printBoard(board, displayBoard, size); // Print the final board with mines revealed
            if (hitMine) {
                printf("Game Over! You hit a mine at %d,%d.\n", row, col);
            } else {
                printf("Congratulations! You've cleared all the mines!\n");
            }
        }
    }

    endTime = clock();
    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC; // Calculate elapsed time in seconds
    printf("Time taken: %.2f seconds\n", timeTaken);

    // Clean up memory
    freeBoard(board, size);
    freeBoard(displayBoard, size);


     // After the game is over, update the leaderboard
    updateLeaderboard(leaderboard, &numEntries, playerName, timeTaken);

    // Display the leaderboard
    displayLeaderboard(leaderboard, numEntries);

    // Save the updated leaderboard to file
    saveLeaderboard(leaderboard, numEntries);

    return 0;
}

// Update the leaderboard with the new player information
void updateLeaderboard(PlayerInfo leaderboard[], int *numEntries, const char *name, double timeTaken) {
    // Insert the new player's information into the leaderboard
    int i;
    for (i = 0; i < *numEntries; i++) {
        if (timeTaken < leaderboard[i].timeTaken) {
            // Shift entries to make room for the new player
            for (int j = *numEntries - 1; j > i; j--) {
                leaderboard[j] = leaderboard[j - 1];
            }
            // Insert the new player
            strcpy(leaderboard[i].name, name);
            leaderboard[i].timeTaken = timeTaken;
            (*numEntries)++;
            break;
        }
    }
    // If the new player has the worst time, but there's still room in the leaderboard
    if (i == *numEntries && *numEntries < MAX_ENTRIES) {
        strcpy(leaderboard[i].name, name);
        leaderboard[i].timeTaken = timeTaken;
        (*numEntries)++;
    }
}

// Display the leaderboard
void displayLeaderboard(const PlayerInfo leaderboard[], int numEntries) {
    printf("\nLeaderboard:\n");
    for (int i = 0; i < numEntries; i++) {
        printf("%d. %s - %.2f seconds\n", i + 1, leaderboard[i].name, leaderboard[i].timeTaken);
    }
}

// Save the leaderboard to a file
void saveLeaderboard(const PlayerInfo leaderboard[], int numEntries) {
    FILE *file = fopen("leaderboard.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < numEntries; i++) {
            fprintf(file, "%s %.2f\n", leaderboard[i].name, leaderboard[i].timeTaken);
        }
        fclose(file);
    }
}

// Load the leaderboard from a file
void loadLeaderboard(PlayerInfo leaderboard[], int *numEntries) {
    FILE *file = fopen("leaderboard.txt", "r");
    if (file != NULL) {
        while (*numEntries < MAX_ENTRIES && fscanf(file, "%s %lf", leaderboard[*numEntries].name, &leaderboard[*numEntries].timeTaken) == 2) {
            (*numEntries)++;
        }
        fclose(file);
    }
}
