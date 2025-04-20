#include <iostream>
#include <mutex>
#include <deque>
#include "frameBuffer.h"

void FrameBuffer::push(const cv::Mat& frame)
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (buffer_.size() >= maxSize_)
	{
		buffer_.pop_front(); // Discard oldest frame
	}
	buffer_.push_back(frame.clone());
}


bool FrameBuffer::pop(cv::Mat& frame)
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (buffer_.empty()) 
		return false;
    frame = buffer_.back().clone();  // Get latest frame
    buffer_.clear();                 // Drop old ones
    return true;
}