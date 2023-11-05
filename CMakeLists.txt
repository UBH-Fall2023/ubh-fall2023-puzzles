cmake_minimum_required(VERSION 3.10)

project(UBHackingFall2023)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/thirdparty/tesseract/include)
add_subdirectory(thirdparty/tesseract)
add_subdirectory(solver)
add_subdirectory(desktop)
