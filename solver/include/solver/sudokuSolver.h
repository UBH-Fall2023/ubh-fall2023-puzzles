#pragma once
#include <array>
#include <vector>
#include <iostream>
#include <algorithm> 

class cell {
public:
	int row;
	int col;
	std::vector<int> possibleNums;
};

class sudokuSolver {

public:
	std::array<std::array<int, 9>, 9> originalGrid;
	std::array<std::array<int, 9>, 9> grid;
	std::vector<cell> list;

	void setGrid(std::array < std::array<int, 9>, 9> g);
	std::array<std::array<int, 9>, 9> getGrid(bool boolean); 
	bool solve();
	bool isSafe(int row, int col, int num);
	void setInitialPossibleNums();
	std::array<std::array<int, 9>, 9> doEverything(std::array<std::array<int, 9>, 9> initialGrid);
};
