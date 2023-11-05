#include <solver/board_detection.h>

cv::Mat prepare_frame(cv::Mat& frame) {
  cv::Mat output;

  // Convert to grayscale.
  cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);

  // Gaussian blur.
  cv::GaussianBlur(output, output, cv::Size(11, 11), 0);

  // Adaptive threshold.
  cv::adaptiveThreshold(output, output, 255,
                        cv::ADAPTIVE_THRESH_MEAN_C | cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                        cv::THRESH_BINARY, 5, 2);

  // Invert the image.
  cv::bitwise_not(output, output);

  // Dilate the image.
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
  cv::dilate(output, output, kernel);

  return output;
}

std::array<cv::Point2f, 4> find_corners(cv::Mat& frame) {
  cv::Mat output = frame.clone();
  // Flood filling to find the biggest blob in the picture.

  // width and height of the frame.
  int width = output.cols,
      height = output.rows;

  cv::Point2i max_point;
  int max_area = 0;

  for (int y = 0; y < height; ++y) {
    uchar* row = output.ptr(y);
    for (int x = 0; x < width; ++x) {
      if (row[x] >= 128) {
        int area = cv::floodFill(output, cv::Point2i(x, y), cv::Scalar(64));
        if (area > max_area) {
          max_area = area;
          max_point = cv::Point2i(x, y);
        }
      }
    }
  }

  // Flood fill the biggest blob with white.
  cv::floodFill(output, max_point, cv::Scalar(255));

  // Erode
  /* cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)); */
  /* cv::erode(output, output, kernel); */

  // Flood fill the rest with black.
  for (int y = 0; y < height; ++y) {
    uchar* row = output.ptr(y);
    for (int x = 0; x < width; ++x) {
      if (row[x] == 64) {
        cv::floodFill(output, cv::Point2i(x, y), cv::Scalar(0));
      }
    }
  }

  // Use "Hough Transform" to detect the lines.
  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(output, lines, 1, CV_PI / 180, 200);

  // Find outermost corners
  cv::Point2f max_x(0, 0),
              max_y(0, 0),
              min_x(width, height),
              min_y(width, height);
  for (auto& line : lines) {
    if (line[0] > max_x.x) {
      max_x.x = line[0];
      max_x.y = line[1];
    }
    if (line[2] > max_x.x) {
      max_x.x = line[2];
      max_x.y = line[3];
    }
    if (line[0] < min_x.x) {
      min_x.x = line[0];
      min_x.y = line[1];
    }
    if (line[2] < min_x.x) {
      min_x.x = line[2];
      min_x.y = line[3];
    }
    if (line[1] > max_y.y) {
      max_y.x = line[0];
      max_y.y = line[1];
    }
    if (line[3] > max_y.y) {
      max_y.x = line[2];
      max_y.y = line[3];
    }
    if (line[1] < min_y.y) {
      min_y.x = line[0];
      min_y.y = line[1];
    }
    if (line[3] < min_y.y) {
      min_y.x = line[2];
      min_y.y = line[3];
    }
  }

  std::array<cv::Point2f, 4> xsorted = {max_x, max_y, min_x, min_y};
  std::sort(xsorted.begin(), xsorted.end(), [](cv::Point2f& a, cv::Point2f& b) {
    return a.x < b.x;
  });

  std::array<cv::Point2f, 4> ysorted = {max_x, max_y, min_x, min_y};
  std::sort(ysorted.begin(), ysorted.end(), [](cv::Point2f& a, cv::Point2f& b) {
    return a.y < b.y;
  });

  cv::Point2f top[2] = {ysorted[0], ysorted[1]};
  cv::Point2f bottom[2] = {ysorted[2], ysorted[3]};
  cv::Point2f left[2] = {xsorted[2], xsorted[3]};
  cv::Point2f right[2] = {xsorted[0], xsorted[1]};

  std::array<cv::Point2f, 4> corners;
  corners[0] = top[0].x < top[1].x ? top[0] : top[1];
  corners[1] = top[0].x > top[1].x ? top[0] : top[1];
  corners[2] = bottom[0].x > bottom[1].x ? bottom[0] : bottom[1];
  corners[3] = bottom[0].x < bottom[1].x ? bottom[0] : bottom[1];

#define CLOSE_PROXIMITY 500

  if (cv::norm(top[0] - top[1]) < CLOSE_PROXIMITY ||
      cv::norm(bottom[0] - bottom[1]) < CLOSE_PROXIMITY ||
      cv::norm(left[0] - left[1]) < CLOSE_PROXIMITY ||
      cv::norm(right[0] - right[1]) < CLOSE_PROXIMITY) {

    corners[0] = cv::Point2f(min_x.x, min_y.y);
    corners[1] = cv::Point2f(max_x.x, min_y.y);
    corners[2] = cv::Point2f(max_x.x, max_y.y);
    corners[3] = cv::Point2f(min_x.x, max_y.y);
  }

  cv::absdiff(frame, output, frame);

  return corners;
}

static cv::Mat crop_to_board(cv::Mat& frame, std::array<cv::Point2f, 4>& _corners) {
  cv::Point2f* corners = _corners.data();

  cv::Mat output;

  cv::Point2f dst[4] = {cv::Point2f(0, 0), cv::Point2f(256, 0), cv::Point2f(256, 256), cv::Point2f(0, 256)};
  cv::Mat transform = cv::getPerspectiveTransform(corners, dst);
  cv::warpPerspective(frame, output, transform, cv::Size(256, 256));

  return output;
}

std::array<cv::Mat, 81> detect_board(cv::Mat& frame) {
  cv::Mat output = prepare_frame(frame);

  std::array<cv::Point2f, 4> corners = find_corners(output);

  frame = crop_to_board(output, corners);

  // split grid into 9x9 grid.
  std::array<cv::Mat, 81> grid_pieces;
  for (int y = 0; y < 9; ++y) {
    for (int x = 0; x < 9; ++x) {

      cv::Rect roi(x * 28, y * 28, 28, 28);
      frame(roi).copyTo(grid_pieces[y * 9 + x]);

      cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(1, 1));
      cv::erode(grid_pieces[y * 9 + x], grid_pieces[y * 9 + x], kernel);
    }
  }

  return grid_pieces;
}
