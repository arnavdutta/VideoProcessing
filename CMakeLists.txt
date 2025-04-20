cmake_minimum_required(VERSION 3.10)

# Compiler
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

project(OpenCVCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# OpenCV
set(OpenCV_DIR "D:/opencv-4.10.0/opencv-4.10.0/build")
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

# Include directories
include_directories(include ${OpenCV_INCLUDE_DIRS})

set(APP_NAME VideoProcessing)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${APP_NAME} ${SOURCES})

target_link_libraries(${APP_NAME} ${OpenCV_LIBS})
