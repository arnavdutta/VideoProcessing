// frameProcessing.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <pipeline/frameProcessing.h>
#include <pipeline/frameBuffer.h>
#include <pipeline/running_flag.h>
#include <processing/getSIFTKeypoints.h>
#include <processing/getCannyEdges.h>


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