/*
 ██████╗██╗ ██╗██████╗  ██████╗ ██╗  ██╗██╗   ██╗    ███████╗ ██████╗ ██╗    ██╗   ██╗███████╗██████╗
██╔════╝██║ ██║██╔══██╗██╔═══██╗██║ ██╔╝██║   ██║    ██╔════╝██╔═══██╗██║    ██║   ██║██╔════╝██╔══██╗
╚█████╗ ██║ ██║██║  ██║██║   ██║█████╔╝ ██║   ██║    ███████╗██║   ██║██║    ██║   ██║█████╗  ██████╔╝
 ╚═══██╗██║ ██║██║  ██║██║   ██║██╔═██╗ ██║   ██║    ╚════██║██║   ██║██║    ╚██╗ ██╔╝██╔══╝  ██╔══██╗
██████╔╝╚█████╔╝██████╔╝╚██████╔╝██║  ██╗╚██████╔╝    ███████║╚██████╔╝███████╗╚████╔╝ ███████╗██║  ██║
╚═════╝  ╚════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝     ╚══════╝ ╚═════╝ ╚══════╝ ╚═══╝  ╚══════╝╚═╝  ╚═╝

  Backtracking Algorithm Implementation in C++
  ============================================
  - 2D array grid representation
  - Row, Column, and 3x3 subgrid constraint checking
  - Recursive backtracking solver
  - Console display of puzzle and solution

  Compile:  g++ -std=c++17 -O2 -o sudoku_solver sudoku_solver.cpp
  Run:      ./sudoku_solver
*/

#include <iostream>
#include <array>
#include <string>

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
constexpr int SIZE  = 9;    // Grid size
constexpr int EMPTY = 0;    // Represents an unfilled cell

// ─────────────────────────────────────────────
//  Type alias – 9×9 grid of ints
// ─────────────────────────────────────────────
using Grid = std::array<std::array<int, SIZE>, SIZE>;

// ─────────────────────────────────────────────
//  Constraint checks
// ─────────────────────────────────────────────

/// Returns true if `num` does NOT already appear in the given row.
bool isValidInRow(const Grid& grid, int row, int num) {
    for (int col = 0; col < SIZE; ++col)
        if (grid[row][col] == num)
            return false;
    return true;
}

/// Returns true if `num` does NOT already appear in the given column.
bool isValidInCol(const Grid& grid, int col, int num) {
    for (int row = 0; row < SIZE; ++row)
        if (grid[row][col] == num)
            return false;
    return true;
}

/// Returns true if `num` does NOT already appear in the 3×3 box
/// that contains the cell (row, col).
bool isValidInBox(const Grid& grid, int row, int col, int num) {
    int boxRowStart = (row / 3) * 3;
    int boxColStart = (col / 3) * 3;
    for (int r = boxRowStart; r < boxRowStart + 3; ++r)
        for (int c = boxColStart; c < boxColStart + 3; ++c)
            if (grid[r][c] == num)
                return false;
    return true;
}

/// Combines all three checks: valid only if `num` satisfies
/// the row, column, AND 3×3 subgrid constraints.
bool isValid(const Grid& grid, int row, int col, int num) {
    return isValidInRow(grid, row, num)
        && isValidInCol(grid, col, num)
        && isValidInBox(grid, row, col, num);
}

// ─────────────────────────────────────────────
//  Backtracking Solver
// ─────────────────────────────────────────────

/**
 * Attempts to solve the Sudoku puzzle using recursive backtracking.
 *
 * Strategy:
 *   1. Scan left-to-right, top-to-bottom for the next empty cell.
 *   2. Try digits 1–9; place a digit if it passes all constraints.
 *   3. Recurse. If recursion returns false, undo (backtrack) and
 *      try the next digit.
 *   4. If no digit works, return false to trigger backtracking above.
 *   5. If no empty cell remains, the puzzle is solved — return true.
 *
 * @param grid  The 9×9 board (modified in-place).
 * @return      true  if a solution was found and stored in grid,
 *              false if the current state leads to no solution.
 */
bool solveSudoku(Grid& grid) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {

            if (grid[row][col] == EMPTY) {            // Found an empty cell
                for (int num = 1; num <= SIZE; ++num) {  // Try 1–9
                    if (isValid(grid, row, col, num)) {
                        grid[row][col] = num;             // Place candidate

                        if (solveSudoku(grid))            // Recurse
                            return true;                  // Solution found!

                        grid[row][col] = EMPTY;           // Backtrack
                    }
                }
                return false; // No valid number → trigger backtracking
            }
        }
    }
    return true; // All cells filled → puzzle solved
}

// ─────────────────────────────────────────────
//  Display Helpers
// ─────────────────────────────────────────────

void printDivider(bool thick = false) {
    std::string seg = thick ? "═══════" : "───────";
    std::string cross = thick ? "╬" : "┼";
    std::string left  = thick ? "╠" : "├";
    std::string right = thick ? "╣" : "┤";
    std::cout << left << seg << cross << seg << cross << seg << right << '\n';
}

void printGrid(const Grid& grid, const std::string& title) {
    std::cout << "\n  ┌───────┬───────┬───────┐\n";
    for (int row = 0; row < SIZE; ++row) {
        if (row == 3 || row == 6) printDivider();
        std::cout << "  │ ";
        for (int col = 0; col < SIZE; ++col) {
            if (col == 3 || col == 6) std::cout << "│ ";
            if (grid[row][col] == EMPTY)
                std::cout << ". ";
            else
                std::cout << grid[row][col] << " ";
        }
        std::cout << "│\n";
    }
    std::cout << "  └───────┴───────┴───────┘\n";
    std::cout << "  " << title << '\n';
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    std::cout << "\n╔══════════════════════════════╗\n";
    std::cout << "║     SUDOKU SOLVER — C++      ║\n";
    std::cout << "╚══════════════════════════════╝\n";

    /*
     * Sample puzzle  (0 = empty)
     *
     *  5 3 . │ . 7 . │ . . .
     *  6 . . │ 1 9 5 │ . . .
     *  . 9 8 │ . . . │ . 6 .
     *  ──────┼───────┼──────
     *  8 . . │ . 6 . │ . . 3
     *  4 . . │ 8 . 3 │ . . 1
     *  7 . . │ . 2 . │ . . 6
     *  ──────┼───────┼──────
     *  . 6 . │ . . . │ 2 8 .
     *  . . . │ 4 1 9 │ . . 5
     *  . . . │ . 8 . │ . 7 9
     */
    Grid puzzle = {{
        {5, 3, 0,  0, 7, 0,  0, 0, 0},
        {6, 0, 0,  1, 9, 5,  0, 0, 0},
        {0, 9, 8,  0, 0, 0,  0, 6, 0},

        {8, 0, 0,  0, 6, 0,  0, 0, 3},
        {4, 0, 0,  8, 0, 3,  0, 0, 1},
        {7, 0, 0,  0, 2, 0,  0, 0, 6},

        {0, 6, 0,  0, 0, 0,  2, 8, 0},
        {0, 0, 0,  4, 1, 9,  0, 0, 5},
        {0, 0, 0,  0, 8, 0,  0, 7, 9}
    }};

    printGrid(puzzle, "Puzzle  (0 = empty)");

    Grid solution = puzzle; // Work on a copy
    if (solveSudoku(solution)) {
        printGrid(solution, "Solution ✓");

        // Verify: every cell must be 1–9
        bool allFilled = true;
        for (auto& row : solution)
            for (int v : row)
                if (v < 1 || v > 9) { allFilled = false; break; }

        std::cout << "\n  Status: " << (allFilled ? "Solved successfully!\n" : "Error in solution.\n");
    } else {
        std::cout << "\n  No solution exists for this puzzle.\n";
    }

    std::cout << '\n';
    return 0;
}
