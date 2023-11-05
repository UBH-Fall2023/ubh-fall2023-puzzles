cmake_minimum_required(VERSION 3.10)

project(UBHackingFall2023)

if(APPLE)
  set(INSTALL_CONFIGS OFF)
  set(DISABLE_TIFF ON)
  set(BUILD_TRAINING_TOOLS ON)
  set(DISABLE_CURL ON)
  set(DISABLE_ARCHIVE ON)
endif()


if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  include_directories(thirdparty/tesseract-ocr-ios/TesseractOCR/include)
  link_directories(thirdparty/tesseract-ocr-ios/TesseractOCR/lib)
else()
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/thirdparty/tesseract/include)
  add_subdirectory(thirdparty/tesseract)
endif()

add_subdirectory(solver)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  add_subdirectory(ios)
else()
  add_subdirectory(desktop)
endif()
