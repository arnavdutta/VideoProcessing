//helloOpencv.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


int main()
{
	std::cout << "Hello OpenCV\n";

	std::cout << "OpenCV version: " << CV_VERSION << std::endl;

	std::cout << "Major: " << CV_MAJOR_VERSION << std::endl;
	std::cout << "Minor: " << CV_MINOR_VERSION << std::endl;
	std::cout << "Revision: " << CV_SUBMINOR_VERSION << std::endl;

	return 0;
}
