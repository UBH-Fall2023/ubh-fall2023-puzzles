#include <solver/solver.h>
#include <solver/board_detection.h>
#include <solver/text_recognition.h>
#include <solver/sudoku_solver.h>

std::optional<SudokuGrid> puzzle_solver(cv::Mat& frame) {
  // Solve stuff here!
  std::array<cv::Mat, 81> cells = detect_board(frame);

  SudokuGrid grid = recognize_board_numbers(cells);

  std::cout << "Original puzzle:" << std::endl;
  print_grid(grid);

  SudokuSolver solver;
  std::optional<SudokuGrid> solved_grid = solver.doEverything(grid);

  if (solved_grid) {
    std::cout << "Solved puzzle:" << std::endl;
    print_grid(*solved_grid);
  }
  else {
    std::cout << "Could not solve puzzle!!" << std::endl;
  }

  return solved_grid;
}
