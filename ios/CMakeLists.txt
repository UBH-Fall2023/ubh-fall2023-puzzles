cmake_minimum_required(VERSION 3.10)

project(CVPuzzleSolver_iOS LANGUAGES CXX Swift)

set(APP_VERSION 1.0.0)
set(APP_VERSION_SHORT 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT XCODE)
  message(FATAL_ERROR "Must use Xcode for building iOS app")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp src/*.mm src/*.swift)
# list(APPEND SOURCES include/UBHackingFall2023-Bridging-Header.h)

#  UBHackingFall2023-Bridging-Header.h
#  UBHackingFall2023-Briging-Header.h


set(CMAKE_Swift_FLAGS "${CMAKE_Swift_FLAGS} -import-objc-header ${CMAKE_CURRENT_SOURCE_DIR}/include/UBHackingFall2023-Bridging-Header.h")

set(APP_BUNDLE_IDENTIFIER    "ubh-fall2023-puzzles")
set(APP_BUNDLE_NAME          "${PROJECT_NAME}")
set(APP_VERSION              "${APP_VERSION}")
set(APP_LONG_VERSION_STRING  "${APP_VERSION}")
set(APP_SHORT_VERSION_STRING "${APP_VERSION_SHORT}")
set(APP_COPYRIGHT            "Copyright © 2023 Peter Lilley and Sam Chen. All rights reserved.")
set(APP_APP_CATEGORY         "public.app-category.games")
set(APP_BUNDLE_ICON_FILE     "bundle_icon.icns")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC solver)
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in" RESOURCE "${RESOURCES}")

set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/tesseract/tessdata/eng.traineddata)
target_sources(${PROJECT_NAME} PUBLIC ${RESOURCES})

set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

