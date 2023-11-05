#include <array>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <solver/sudoku_solver.h>

bool compareTwoCells(Cell cell1, Cell cell2) {
  return (cell1.possibleNums.size() < cell2.possibleNums.size());
}

void SudokuSolver::setGrid(std::array < std::array<int, 9>, 9> g) {
  grid = g;
}

bool SudokuSolver::solve() {
  if (list.empty()) {
    return true;
  }
  std::sort(list.begin(), list.end(), compareTwoCells);
  Cell temp = list.at(0);
  for (int i = 0; i < temp.possibleNums.size(); i++) {
    if (isSafe(temp.row, temp.col, temp.possibleNums.at(i))) {
      grid[temp.row][temp.col] = temp.possibleNums.at(i);
      list.erase(list.begin());
      if (solve()){
        return true;
      }
      list.insert(list.begin(), temp);
      std::sort(list.begin(), list.end(), compareTwoCells);
      grid[list.at(0).row][list.at(0).col] = 0;
    }
  }
  return false;

}

bool SudokuSolver::isSafe(int row, int col, int num) {
  for (int i = 0; i < 9; i++) {
    if (grid[row][i] == num && i!=col) {
      return false;
    }
    if (grid[i][col] == num && i!=row) {
      return false;
    }
  }
  int boxRow = floor(row/3) * 3;
  int boxCol = floor(col/3) * 3; 
  for (int i = boxRow; i < boxRow + 3; i++) {
    for (int j = boxCol; j < boxCol + 3; j++) {
      if (grid[i][j] == num && !(i == row && j == col)) {
        return false;
      }
    }
  }
  return true;
}

void SudokuSolver::setInitialPossibleNums() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (grid[i][j] == 0) {
        Cell cell;
        cell.row = i;
        cell.col = j;
        for (int x = 1; x <= 9; x++) {
          if (isSafe(i, j, x)) {
            cell.possibleNums.push_back(x);
          }
        }
        list.push_back(cell);
      }
    }
  }
}

std::optional<SudokuGrid> SudokuSolver::doEverything(SudokuGrid initialGrid) {
  setGrid(initialGrid);
  setInitialPossibleNums();
  bool res = solve();
  if (res) {
    return grid;
  }
  return std::nullopt;
}
