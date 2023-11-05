#include <array>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <solver/sudokuSolver.h>



std::array<std::array<int, 9>, 9> originalGrid;
std::array<std::array<int, 9>, 9> grid;
std::vector<cell> list;


bool compareTwoCells(cell cell1, cell cell2) {
	return (cell1.possibleNums.size() < cell2.possibleNums.size());
}

void sudokuSolver::setGrid(std::array < std::array<int, 9>, 9> g) {
	grid = g;
	originalGrid = g;
}

std::array<std::array<int, 9>, 9> sudokuSolver::getGrid(bool boolean) {
	if (boolean) {
		return grid;
	}
	else {
		std::cout << "Puzzle is unsolvable" << std::endl;
		return originalGrid;
	}
}

bool sudokuSolver::solve() {
	if (list.empty()) {
		return true;
	}
	std::sort(list.begin(), list.end(), compareTwoCells);
	cell temp = list.at(0);
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

bool sudokuSolver::isSafe(int row, int col, int num) {
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

void sudokuSolver::setInitialPossibleNums() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (grid[i][j] == 0) {
				cell cell;
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

std::array<std::array<int, 9>, 9> sudokuSolver::doEverything(std::array<std::array<int, 9>, 9> initialGrid) {
	setGrid(initialGrid);
	setInitialPossibleNums();
	bool boolean = solve();
	std::array<std::array<int, 9>, 9> ans = getGrid(boolean);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cout << grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return ans;
}



