//getSIFTKeypoints.cpp

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// Function to get SIFT keypoints and descriptors from a frame
std::pair<std::vector<cv::KeyPoint>, cv::Mat> getSIFTKeypoints(const cv::Mat& frame, bool debug=false)
{
    // Check if the frame is empty
    if (frame.empty())
    {
        std::cerr << "Empty frame received." << std::endl;
        return {};
    }
    // Convert frame to grayscale
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    
    // Detect keypoints using SIFT
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
    sift->detectAndCompute(grayFrame, cv::Mat(), keypoints, descriptors);
    
    if (debug)
    {
        // Draw keypoints on the frame
        cv::Mat keypointFrame;
        cv::drawKeypoints(frame, keypoints, keypointFrame, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        // Display the frame with keypoints
        cv::imshow("Keypoints", keypointFrame);

        std::cout << "Number of keypoints detected: " << keypoints.size() << std::endl;
        std::cout << "Descriptors size: " << descriptors.size() << std::endl;
    }

    // Return the keypoints and descriptors as a pair
    return {keypoints, descriptors};
}
