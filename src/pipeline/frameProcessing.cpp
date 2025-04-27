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
                std::vector<cv::DMatch> goodMatches;
                for (const auto& knnMatch : knnMatches)
                {
                    if (knnMatch.size() == 2 && knnMatch[0].distance < ratio_thresh * knnMatch[1].distance)
                        goodMatches.push_back(knnMatch[0]);
                }

                if (goodMatches.size() >= 8) // essential matrix needs at least 8 points
                {
                    std::vector<cv::Point2f> pts1, pts2;
                    for (auto& match : goodMatches)
                    {
                        pts1.push_back(kpStored[match.queryIdx].pt);
                        pts2.push_back(kpCurrent[match.trainIdx].pt);
                    }

                    // Find fundamental matrix to filter out outliers
                    std::vector<uchar> inliersMask(pts1.size());
                    cv::Mat F = cv::findFundamentalMat(pts1, pts2, cv::FM_RANSAC, 3.0, 0.99, inliersMask);

                    std::vector<cv::Point2f> inlierPts1, inlierPts2;
                    for (size_t i = 0; i < inliersMask.size(); i++)
                    {
                        if (inliersMask[i])
                        {
                            inlierPts1.push_back(pts1[i]);
                            inlierPts2.push_back(pts2[i]);
                        }
                    }

                    if (inlierPts1.size() >= 4)
                    {
                        cv::Mat homography, maskHomography;
                        homography = cv::findHomography(inlierPts2, inlierPts1, cv::RANSAC, 4.0, maskHomography, 2000, 0.995);
                        // Notice: inlierPts2 -> inlierPts1

                        if (!homography.empty() && homography.rows == 3 && homography.cols == 3)
                        {
                            if (homography.type() != CV_32F && homography.type() != CV_64F)
                                homography.convertTo(homography, CV_32F);

                            cv::Mat transformedFrame;
                            cv::warpPerspective(currentFrame, transformedFrame, homography, storedFrame.size());
                            // Notice: warp CURRENT frame to align with STORED frame

                            cv::Mat overlay;
                            cv::addWeighted(storedFrame, 0.5, transformedFrame, 0.5, 0, overlay);

                            cv::imshow("Overlay: Stored vs Aligned Current", overlay);
                        }
                        else
                        {
                            std::cerr << "Error: Invalid homography matrix." << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Error: Not enough inliers after fundamental matrix filtering." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error: Not enough good matches for fundamental matrix estimation." << std::endl;
                }
            }

            // Display the current frame
            cv::imshow("Current Frame", currentFrame);
        }

        char key = (char)cv::waitKey(1);
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
                std::cout << "Stored frame index: " << storedFrameIndex << std::endl;
            }
            else
            {
                // Capture the latest frame and show the stitched output
                storedFrame.release();
                isFrameStored = false;
                std::cout << "Reset stored frame." << std::endl;
            }
        }
    }
}

