#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <VideoProcessing/frameProcessing.h>
#include <VideoProcessing/frameBuffer.h>
#include <VideoProcessing/running_flag.h>


void frameProcessing(FrameBuffer& fb)
{
	while(running)
	{
		cv::Mat frame;
		if (fb.pop(frame))
		{
			// Simulate processing: Canny edge detection
			cv::Mat edges;
			cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
			cv::Canny(frame, edges, 100, 200);
			cv::imshow("Processed", edges);
		}

		if (cv::waitKey(1) == 'q')
		{
			running = false;
			break;
		}
	}
}