// getSIFTKeypoints.hpp

#pragma once

#include <opencv2/opencv.hpp>

std::pair<std::vector<cv::KeyPoint>, cv::Mat> getSIFTKeypoints(const cv::Mat&, bool);