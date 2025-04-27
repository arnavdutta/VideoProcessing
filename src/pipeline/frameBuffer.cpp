// frameBuffer.cpp

#include <iostream>
#include <mutex>
#include <deque>
#include <pipeline/frameBuffer.hpp>

void FrameBuffer::push(const cv::Mat& frame)
{
	Frame newFrame;
	newFrame.frame = frame.clone();
	newFrame.frameIndex = ++frameIdx_;

	std::unique_lock<std::mutex> lock(mutex_);
	if (buffer_.size() >= maxSize_)
	{
		buffer_.pop_front(); // Discard oldest frame
	}
	buffer_.push_back(newFrame);
	cond_.notify_one();
}


bool FrameBuffer::pop(Frame& outFrame)
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] 
		{ return !buffer_.empty(); });
	if (buffer_.empty()) 
		return false;
	outFrame = buffer_.back();		// Get latest frame
    buffer_.clear();                // Drop old ones
    return true;
}

Frame FrameBuffer::getFrameByIndex(uint32_t idx)
{
	std::unique_lock<std::mutex> lock(mutex_);
	for (auto& f : buffer_)
	{
		if (f.frameIndex == idx)
		{
			return f;
		}
	}
	return Frame{}; // Return an empty Frame if not found
}