// frameProcessing.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <pipeline/frameProcessing.hpp>
#include <pipeline/frameBuffer.hpp>
#include <pipeline/running_flag.hpp>
#include <processing/getSIFTKeypoints.hpp>
#include <processing/getCannyEdges.hpp>


void frameProcessing(FrameBuffer& fb)
{
	while(running)
	{
		cv::Mat frame;
		if (fb.pop(frame))
		{
			getCannyEdges(frame);
			getSIFTKeypoints(frame);
		}

		if (cv::waitKey(1) == 'q')
		{
			running = false;
			cv::destroyAllWindows();
			break;
		}
	}
}