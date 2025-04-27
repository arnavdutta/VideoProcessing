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
                // Get SIFT keypoints and descriptors for both frames
                std::vector<cv::KeyPoint> kpStored, kpCurrent;
                cv::Mat descStored, descCurrent;

                std::tie(kpStored, descStored) = getSIFTKeypoints(storedFrame, true);
                std::tie(kpCurrent, descCurrent) = getSIFTKeypoints(currentFrame, true);

                // Match keypoints
                // FLANN Matcher(KD - Trees automatically)
                cv::Ptr<cv::DescriptorMatcher> matcher = cv::FlannBasedMatcher::create();
                
                // kNN Match(k = 2)
                std::vector<std::vector<cv::DMatch>> knnMatches;
                matcher->knnMatch(descStored, descCurrent, knnMatches, 2);


                // Lowe’s Ratio Test
                const float ratio_thresh = 0.75f;
                std::vector<cv::DMatch> good_matches;
                for (const auto& knnMatch : knnMatches) 
                {
                    if (knnMatch.size() == 2 && knnMatch[0].distance < ratio_thresh * knnMatch[1].distance) 
                    {
                        good_matches.push_back(knnMatch[0]);
                    }
                }

				// Find homography matrix
				std::vector<cv::Point2f> pointsStored, pointsCurrent;
				for (const auto& match : good_matches)
				{
					pointsStored.push_back(kpStored[match.queryIdx].pt);
					pointsCurrent.push_back(kpCurrent[match.trainIdx].pt);
				}
				
                if (pointsStored.size() >= 4 && pointsCurrent.size() >= 4)
				{
					cv::Mat homography = cv::findHomography(pointsStored, pointsCurrent, cv::RANSAC);
                    
                    if (homography.empty()) 
                    {
                        std::cerr << "Error: Homography computation failed." << std::endl;
                        continue;
                    }

                    if (homography.rows != 3 || homography.cols != 3) 
                    {
                        std::cerr << "Error: Homography matrix is not 3x3." << std::endl;
                        continue;
                    }
                    if (homography.type() != CV_32F && homography.type() != CV_64F) 
                    {
                        std::cerr << "Error: Homography matrix has wrong type. Converting to CV_32F..." << std::endl;
                        homography.convertTo(homography, CV_32F);
                    }
				
                    // Perform perspective transform
					cv::Mat transformedFrame;
					cv::warpPerspective(storedFrame, transformedFrame, homography, currentFrame.size());
					
                    // Display the stitched output
					cv::Mat stitchedOutput;
					cv::addWeighted(currentFrame, 0.5, transformedFrame, 0.5, 0, stitchedOutput);
					cv::imshow("Stitched Output", stitchedOutput);

                    // Draw matches (optional)
                    cv::Mat img_matches;
                    cv::drawMatches(storedFrame, kpStored, currentFrame, kpCurrent, good_matches, img_matches);
                    cv::imshow("Good Matches", img_matches);
				}
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
