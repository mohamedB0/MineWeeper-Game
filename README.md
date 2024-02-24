# Minesweeper Game
![image](https://github.com/mohamedB0/GUI_GTK4/assets/115374122/9e46e913-468d-4eeb-aa73-9a664fb74df9)


This Minesweeper game is a console-based version of the classic puzzle game. The player's goal is to clear a rectangular board containing hidden "mines" without detonating any of them, using clues about the number of neighboring mines in each field.

## Features

- Three difficulty levels: Beginner, Intermediate, and Expert.
- Ability to place flags on cells where mines are suspected.
- Reveals adjacent cells automatically when a cell with zero adjacent mines is revealed.
- Game over detection when a mine is revealed.
- Win detection when all non-mine cells are revealed.

## Getting Started

### Prerequisites

- GCC compiler or any standard C compiler.

### Compiling the Game

To compile the Minesweeper game, navigate to the directory containing the game's source code and run the following command:

```bash
gcc -o minesweeper minesweeper.c
```

This command will compile the source code into an executable named `minesweeper`.

### Running the Game

After compiling, you can start the game by running:

```bash
./minesweeper
```

## How to Play

1. **Select Difficulty**: At the start of the game, choose a difficulty level (`B` for Beginner, `I` for Intermediate, `E` for Expert).
2. **Reveal Cells**: Enter row and column numbers to reveal a cell (e.g., `3 4`).
3. **Place/Remove Flags**: Enter `f` followed by row and column numbers to place or remove a flag on a cell (e.g., `f 3 4`). Flags can be used to mark cells you suspect contain mines.
4. **Winning the Game**: The game is won by revealing all cells that do not contain mines.
5. **Game Over**: The game ends if you reveal a cell containing a mine.

## Code Structure

- **Initialization**: The game board is dynamically allocated based on the selected difficulty level, and mines are placed randomly.
- **Gameplay Loop**: The game prompts the player for input and updates the game state until the player wins or hits a mine.
- **Memory Management**: Dynamically allocated memory for the game board is properly freed at the end of the game to prevent memory leaks.

## Contributing

Feel free to fork this project and submit pull requests if you have suggestions for improvements or new features.

## License

This project is open-source and available under the [MIT License](LICENSE).

