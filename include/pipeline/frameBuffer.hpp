// frameBuffer.hpp

#pragma once

#include <iostream>
#include <mutex>
#include <deque>
#include <opencv2/opencv.hpp>

class FrameBuffer
{
private:
    std::deque<cv::Mat> buffer_;
    std::mutex mutex_;
    size_t maxSize_;	

public:
	FrameBuffer(size_t MAX_SIZE = 1):maxSize_(MAX_SIZE){}

	void push(const cv::Mat& frame);

	bool pop(cv::Mat& frame);
};