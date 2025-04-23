// getCannyEdges.cpp

#include <iostream>
#include <opencv2/opencv.hpp>

void getCannyEdges(cv::Mat& frame)
{
	// Check if the frame is empty
	if (frame.empty())
	{
		std::cerr << "Empty frame received." << std::endl;
		return;
	}
	// Convert frame to grayscale and apply Canny edge detection
	cv::Mat edges;
	cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
	cv::Canny(edges, edges, 50, 200);
	
	// Display the processed frame
	cv::imshow("Edges", edges);
}
			
			
			