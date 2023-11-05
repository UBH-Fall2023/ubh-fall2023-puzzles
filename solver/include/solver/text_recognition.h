#pragma once

#include <opencv2/opencv.hpp>

#include <array>

std::array<std::array<int, 9>, 9> recognize_board_numbers(std::array<cv::Mat, 81>& frame);

/* std::string recognize_text(cv::Mat& frame); */
