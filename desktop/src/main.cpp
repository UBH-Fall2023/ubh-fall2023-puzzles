#include <solver/solver.h>

#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {
  // Open the camera.
  cv::VideoCapture capture(0);
  if (!capture.isOpened()) {
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }

  cv::Mat frame, output_frame;
  while (1) {
    // Capture the frame.
    capture >> frame;
    if (frame.empty()) {
      std::cout << "No captured frame" << std::endl;
      break;
    }

    output_frame = puzzle_solver(frame);

    // Present the frame.
    cv::imshow("Frame", output_frame);

    // Quit if ESC pressed.
    char c = (char)cv::waitKey(25);
    if (c == 27) break;
  }

  return 0;
}
