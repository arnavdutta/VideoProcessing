//cameraCapture.cpp

#include <thread>
#include <opencv2/opencv.hpp>
#include <pipeline/frameBuffer.hpp>
#include <pipeline/running_flag.hpp>


// Simple Program

// void cameraCapture()
// {
//     cv::Mat frame;
    
//     //--- INITIALIZE VIDEOCAPTURE
//     cv::VideoCapture cap;
    
//     // open the default camera using default API
//     // cap.open(0);
//     // OR advance usage: select any API backend
//     int deviceID = 0;             // 0 = open default camera
//     int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    
//     // open selected camera using selected API
//     cap.open(deviceID, apiID);
    
//     // check if we succeeded
//     if (!cap.isOpened()) 
//     {
//         std::cerr << "[ERROR] Unable to open camera\n";
//     }

//     //--- GRAB AND WRITE LOOP
//     std::cout << "Start camera capturing..." << std::endl
//         << "Press any key to terminate" << std::endl;
//     for (;;)
//     {
//         // wait for a new frame from camera and store it into 'frame'
//         cap.read(frame);
//         // check if we succeeded
        
//         if (frame.empty()) 
//         {
//             std::cerr << "[ERROR] Blank frame captured\n";
//             break;
//         }
//         // show live and wait for a key with timeout long enough to show images
//         cv::imshow("Live", frame);
//         if (cv::waitKey(5) >= 0)
//             break;
//     }
// }


void cameraCapture(FrameBuffer& fb)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "Camera open failed\n";
        running = false;
        return;
    }

    while(running)
    {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
            continue;
        fb.push(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate 100 FPS max
    }

    cap.release();
}