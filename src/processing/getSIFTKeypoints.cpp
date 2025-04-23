//getSIFTKeypoints.cpp

#include <iostream>
#include <opencv2/opencv.hpp>
	

// Function to get SIFT keypoints from a frame
void getSIFTKeypoints(cv::Mat& frame)
{
	// Check if the frame is empty
	if (frame.empty())
	{
		std::cerr << "Empty frame received." << std::endl;
		return;
	}
	// Convert frame to grayscale
	cv::Mat grayFrame;
	cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
	
	// Detect keypoints using SIFT
	std::vector<cv::KeyPoint> keypoints;
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
	sift->detect(grayFrame, keypoints);
	
	// Draw keypoints on the frame
	cv::Mat keypointFrame;
	cv::drawKeypoints(frame, keypoints, keypointFrame, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
	// Display the frame with keypoints
	cv::imshow("Keypoints", keypointFrame);
}