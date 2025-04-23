// main.cpp

#include <thread>
#include <opencv2/opencv.hpp>
#include <pipeline/frameBuffer.hpp>
#include <pipeline/frameProcessing.hpp>
#include <pipeline/cameraCapture.hpp>
#include <pipeline/running_flag.hpp>




int main()
{
    FrameBuffer buffer(1);  // Single frame buffer for minimal latency

    std::thread t1(cameraCapture, std::ref(buffer));
    std::thread t2(frameProcessing, std::ref(buffer));

    t1.join();
    t2.join();
    
    cv::destroyAllWindows();

    return 0;
}