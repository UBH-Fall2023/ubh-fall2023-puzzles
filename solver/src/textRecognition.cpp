#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <tesseract/baseapi.h>



std::string textRecognition(cv::Mat image) {
	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	tess.SetImage(image.data, image.cols, image.rows, 1, image.step);
	std::string text = std::string(tess.GetUTF8Text());
	std::cout << text;
	tess.End();
	return text;
	
	

	
	

};