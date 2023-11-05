#include <solver/solver.h>
#include <solver/board_detection.h>

#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {

#if 0
  // Open the camera.
  cv::VideoCapture capture(0);
  if (!capture.isOpened()) {
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }

  cv::Mat frame, preview_frame;
  while (1) {
    // Capture the frame.
    capture >> frame;
    if (frame.empty()) {
      std::cout << "No captured frame" << std::endl;
      break;
    }

    preview_frame = prepare_frame(frame);
    std::array<cv::Point2f, 4> corners = find_corners(preview_frame);
    cv::line(frame, corners[0], corners[1], cv::Scalar(0, 255, 0), 4);
    cv::line(frame, corners[1], corners[2], cv::Scalar(0, 255, 0), 4);
    cv::line(frame, corners[2], corners[3], cv::Scalar(0, 255, 0), 4);
    cv::line(frame, corners[3], corners[0], cv::Scalar(0, 255, 0), 4);

    // Present the frame.
    cv::imshow("Frame", frame);

    // Quit if ESC pressed.
    char c = (char)cv::waitKey(25);
    if (c == 27) break;
  }
#else

  /* cv::imwrite("output.png", frame); */
  cv::Mat frame = cv::imread("output.png");
#endif

  // Now do processing on the frame!
  std::optional<SudokuGrid> grid = puzzle_solver(frame);
  (void)grid;

  return 0;
}
