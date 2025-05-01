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
#include <processing/performImageRegistration.hpp>
//#include <processing/getCannyEdges.hpp>


void frameProcessing(FrameBuffer& fb)
{
    cv::Mat storedFrame;
    uint32_t storedFrameIndex = 0;
    bool isFrameStored = false;

    while (running)
    {
        Frame frame;
        if (fb.pop(frame))
        {
            cv::Mat currentFrame = frame.frame;
            uint32_t currentFrameIndex = frame.frameIndex;

            // If a frame is stored, perform keypoint matching and perspective transform
            if (isFrameStored)
            {
                cv::Mat transformedFrame = performImageRegistration(storedFrame, currentFrame, true);
                if (transformedFrame.empty())
                {
                    std::cerr << "Failed to transform frame." << std::endl;
                }
                // Display the stitched output
                cv::Mat stitchedOutput;
                cv::addWeighted(currentFrame, 0.5, transformedFrame, 0.5, 0, stitchedOutput);
                cv::imshow("Stitched Output", stitchedOutput);                
            }

            // Display the current frame
            cv::imshow("Current Frame", currentFrame);
        }

        // Check for key press
        char key = cv::waitKey(1);
        if (key == 'q')
        {
            running = false;
            cv::destroyAllWindows();
            break;
        }
        else if (key == 'c')
        {
            // Store the current frame and its index
            if (!isFrameStored)
            {
                storedFrame = frame.frame.clone();
                storedFrameIndex = frame.frameIndex;
                isFrameStored = true;
                std::cout << "Frame stored: Index " << storedFrameIndex << std::endl;
            }
            else
            {
                // Capture the latest frame and show the stitched output
                storedFrame.release();
                isFrameStored = false;
                std::cout << "Stitched output displayed. Frame storage reset." << std::endl;
            }
        }
    }
}
