#pragma once
#include <vector>
#include <solver/sudoku_grid.h>
#include <optional>

class Cell {
public:
  int row;
  int col;
  std::vector<int> possibleNums;
};

class SudokuSolver {
  SudokuGrid grid;
  std::vector<Cell> list;

public:
  void setGrid(SudokuGrid g);
  bool solve();
  bool isSafe(int row, int col, int num);
  void setInitialPossibleNums();
  std::optional<SudokuGrid> doEverything(SudokuGrid initialGrid);
};
