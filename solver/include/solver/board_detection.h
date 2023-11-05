#pragma once

#include <opencv2/opencv.hpp>
#include <array>

cv::Mat prepare_frame(cv::Mat& frame);
std::array<cv::Point2f, 4> find_corners(cv::Mat& frame);

std::array<cv::Mat, 81> detect_board(cv::Mat& frame);
