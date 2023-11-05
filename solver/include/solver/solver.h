#pragma once

#include <opencv2/opencv.hpp>
#include <solver/sudoku_grid.h>

std::optional<SudokuGrid> puzzle_solver(cv::Mat& frame);
