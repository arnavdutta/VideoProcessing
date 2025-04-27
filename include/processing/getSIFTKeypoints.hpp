// getSIFTKeypoints.hpp

#pragma once

#include <opencv2/opencv.hpp>

std::pair<std::vector<cv::KeyPoint>, cv::Mat> getSIFTKeypoints(cv::Mat&, bool);