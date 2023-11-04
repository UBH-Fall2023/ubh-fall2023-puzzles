cmake_minimum_required(VERSION 3.10)

project(UBHackingFall2023)

set(DISABLE_TIFF ON)
set(BUILD_TRAINING_TOOLS OFF)
add_subdirectory(thirdparty/tesseract)
add_subdirectory(solver)
add_subdirectory(desktop)
