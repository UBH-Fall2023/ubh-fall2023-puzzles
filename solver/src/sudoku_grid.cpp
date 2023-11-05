#include <solver/sudoku_grid.h>
#include <iostream>

void print_grid(const SudokuGrid& grid) {
  for (int x = 0; x < 9; ++x) {
    std::cout << '[';
    for (int y = 0; y < 9; ++y) {
      if (grid[x][y]) {
        std::cout << grid[x][y];
      } else {
        std::cout << ' ';
      }
      std::cout << ' ';
    }
    std::cout << ']' << std::endl;
  }
}

