# How to build the project

1. Install CMake v4.0.0
2. Add CMake path to environment variables (C:\Program Files\CMake\bin)
3. Install MinGW (x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z). (From - https://github.com/2641797006/c/blob/master/GCC-zip/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z)
4. Add MinGW path to environment variables (C:\mingw64\bin)
5. Download OpenCV v4.10.0(https://github.com/opencv/opencv/archive/refs/tags/4.10.0.zip)
6. Extract and goto OpenCV directory and create a build folder.
7. Goto build folder and build OpenCV using MingW.
	* mkdir build
	* cd build
	* cmake .. -G "MinGW Makefiles"
	* cmake --build .
8. After build is completed, add lib and bin to environment variables. (D:\opencv-4.10.0\opencv-4.10.0\build\bin, D:\opencv-4.10.0\opencv-4.10.0\build\lib)
9. Now, all set to build your OpenCV project. Create a build folder inside your project root directory and build it using the set of commands used to build the OpenCV.
