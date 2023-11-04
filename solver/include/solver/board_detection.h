#pragma once

#include <opencv2/opencv.hpp>
#include <array>

std::array<cv::Mat, 81> detect_board(cv::Mat& frame);
