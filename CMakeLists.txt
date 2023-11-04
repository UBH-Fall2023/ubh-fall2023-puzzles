cmake_minimum_required(VERSION 3.10)

project(UBHackingFall2023)

set(DISABLE_TIFF ON)
set(BUILD_TRAINING_TOOLS OFF)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/thirdparty/tesseract/include)
add_subdirectory(thirdparty/tesseract)
add_subdirectory(solver)
add_subdirectory(desktop)
