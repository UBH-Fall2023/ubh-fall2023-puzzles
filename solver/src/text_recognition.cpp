#include <solver/text_recognition.h>
#include <tesseract/baseapi.h>

static tesseract::TessBaseAPI g_tess;

static bool can_ignore_char(char c) {
  return c == '1' || c == '|' || c == 'i' || c == 'l' || c == '[' || c == ']';
}

static int convert_char(char c) {
  if (c == 'Q' || c == 'g') {
    return 9;
  }

  if (c >= '1' && c <= '9') {
    return c - '0';
  }
  return 0;
}

int recognize_number(cv::Mat& frame) {
  g_tess.SetImage(frame.data, frame.cols, frame.rows, 1, frame.cols);

  const char* text = g_tess.GetUTF8Text();
  
  /* while (1) { */
  /*   cv::imshow("Frame", frame); */
  /*   if (cv::waitKey(25) == 27) break; */
  /* } */

  if (!text) {
    return 0;
  }

  if (*text != 'a') {
    std::string text_str = text;
    text_str.erase(std::remove(text_str.begin(), text_str.end(), '\n'), text_str.end());

    std::cout << "Text: " << text_str << std::endl;
  }

  if (strlen(text) == 1) {
    return convert_char(*text);
  }

  if (can_ignore_char(text[0]) && convert_char(text[1])) {
    return convert_char(text[1]);
  }

  if (can_ignore_char(text[1]) && convert_char(text[0])) {
    return convert_char(text[0]);
  }

  g_tess.Clear();

  return atoi(text);
}

std::array<std::array<int, 9>, 9> recognize_board_numbers(std::array<cv::Mat, 81>& frame) {
  g_tess.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
  g_tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

  std::array<std::array<int, 9>, 9> grid;

  for (int i = 0; i < 81; i++) {
    int number = recognize_number(frame[i]);
    std::cout << "Number: " << number << std::endl;
    grid[i / 9][i % 9] = number;
  }

  g_tess.End();

  return grid;
}
