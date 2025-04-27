// frameBuffer.hpp

#pragma once

#include <iostream>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <opencv2/opencv.hpp>


struct Frame
{
    cv::Mat frame;
    uint32_t frameIndex;
};

class FrameBuffer
{
private:
    std::deque<Frame> buffer_;
    std::mutex mutex_;
    size_t maxSize_;
    std::condition_variable cond_;
	uint32_t frameIdx_;

public:
	FrameBuffer(size_t MAX_SIZE = 10):maxSize_(MAX_SIZE), frameIdx_(0) {}

	void push(const cv::Mat& frame);

	bool pop(Frame& outFrame);

    Frame getFrameByIndex(uint32_t idx);
};
