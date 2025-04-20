#include <thread>
#include <opencv2/opencv.hpp>
#include <VideoProcessing/frameBuffer.h>
#include <VideoProcessing/frameProcessing.h>
#include <VideoProcessing/cameraCapture.h>
#include <VideoProcessing/running_flag.h>




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