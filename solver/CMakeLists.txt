cmake_minimum_required(VERSION 3.10)

project(solver)

if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
  set(CMAKE_CXX_FLAGS "-x objective-c++")
endif()



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
  # target_link_libraries(${PROJECT_NAME} PUBLIC "-framework OpenCV")
else()
  find_package(OpenCV REQUIRED)
  if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV not found")
  endif()

  target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
  target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  target_link_libraries(${PROJECT_NAME} PUBLIC libtesseract.a liblept.a libjpeg.a libpng.a libtiff.a)
  target_compile_definitions(${PROJECT_NAME} PUBLIC SOLVER_IOS)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC libtesseract)
endif()
