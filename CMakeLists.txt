cmake_minimum_required(VERSION 3.10)

project(OpenCVCPP)

set(APP_NAME VideoProcessing)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenCV_DIR "D:/opencv-4.10.0/opencv-4.10.0/build")
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

set(SOURCES
    main.cpp
    cameraCapture.cpp
    frameBuffer.cpp
    frameProcessing.cpp
    running_flag.cpp
)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${APP_NAME} ${SOURCES})

target_link_libraries(${APP_NAME} ${OpenCV_LIBS})
