#import <solver_bridge.h>
#include <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>
#import <Foundation/Foundation.h>
#include <solver/board_detection.h>
#include <solver/solver.h>

@implementation PuzzleSolverBridge
    
- (UIImage *) detectBoardIn: (UIImage *) image {
  cv::Mat image_mat;
  UIImageToMat(image, image_mat, true);
  if (image_mat.empty()) {
    return nullptr;
  }
  
  cv::cvtColor(image_mat, image_mat, cv::COLOR_RGBA2BGR);
    
  cv::Mat frame = prepare_frame(image_mat);
  std::array<cv::Point2f, 4> corners = find_corners(frame);
  cv::line(image_mat, corners[0], corners[1], cv::Scalar(0, 255, 0), 4);
  cv::line(image_mat, corners[1], corners[2], cv::Scalar(0, 255, 0), 4);
  cv::line(image_mat, corners[2], corners[3], cv::Scalar(0, 255, 0), 4);
  cv::line(image_mat, corners[3], corners[0], cv::Scalar(0, 255, 0), 4);

  
  return MatToUIImage(image_mat);
}

- (NSArray *) solvePuzzle: (UIImage *) image {
  cv::Mat image_mat;
  UIImageToMat(image, image_mat, true);

  std::optional<SudokuGrid> _grid = puzzle_solver(image_mat);
  if (!_grid) {
    return nullptr;
  }

  SudokuGrid grid = _grid.value();

  NSMutableArray *result = [[NSMutableArray alloc] init];
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) {
      [result addObject: [NSNumber numberWithInt: grid[x][y]]];
    }
  }

  return result;
}
    
@end
