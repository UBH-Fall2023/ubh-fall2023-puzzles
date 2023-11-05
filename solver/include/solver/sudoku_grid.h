#pragma once

#include <array>

using SudokuGrid = std::array<std::array<int, 9>, 9>;

void print_grid(const SudokuGrid& grid);
