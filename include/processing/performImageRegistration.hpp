// performImageRegistration.hpp

#pragma once

#include <opencv2/opencv.hpp>

cv::Mat performImageRegistration(const cv::Mat& storedFrame, cv::Mat& currentFrame, bool debug);