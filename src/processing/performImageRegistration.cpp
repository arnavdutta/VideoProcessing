#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <opencv2/opencv.hpp>
#include <processing/getSIFTKeypoints.hpp>


cv::Mat performImageRegistration(const cv::Mat& storedFrame, cv::Mat& currentFrame, bool debug)
{
    // Get SIFT keypoints and descriptors for both frames
    std::vector<cv::KeyPoint> kpStored, kpCurrent;
    cv::Mat descStored, descCurrent;
    cv::Mat transformedFrame;

    std::tie(kpStored, descStored) = getSIFTKeypoints(storedFrame, true);
    std::tie(kpCurrent, descCurrent) = getSIFTKeypoints(currentFrame, true);

    // Match keypoints
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

            if (!homography.empty() && homography.rows == 3 && homography.cols == 3)
            {
                if (homography.type() != CV_32F && homography.type() != CV_64F)
                    homography.convertTo(homography, CV_32F);

                cv::warpPerspective(currentFrame, transformedFrame, homography, storedFrame.size());

                cv::Mat overlay;
                cv::addWeighted(storedFrame, 0.5, transformedFrame, 0.5, 0, overlay);

                if (debug)
                {
                    cv::imshow("Overlay: Stored vs Aligned Current", overlay);
                    // Draw matches (optional)
                    cv::Mat img_matches;
                    cv::drawMatches(storedFrame, kpStored, currentFrame, kpCurrent, goodMatches, img_matches);
                    cv::imshow("Good Matches", img_matches);
                }
                
            }
            else  
            {  
                std::cerr << "Error: Invalid homography matrix." << std::endl;  
                return cv::Mat();  
            }  
        }  
        else  
        {  
            std::cerr << "Error: Not enough inliers after fundamental matrix filtering." << std::endl;  
            return cv::Mat();  
        }  
    }  
    else  
    {  
        std::cerr << "Error: Not enough good matches for fundamental matrix estimation." << std::endl;  
        return cv::Mat();  
    }

    // Use the transformed image from warpPerspective for optical flow registration
    // Convert transformedFrame and storedFrame to grayscale for optical flow
    cv::Mat grayTransformed, grayStored;
    cv::cvtColor(transformedFrame, grayTransformed, cv::COLOR_BGR2GRAY);
    cv::cvtColor(storedFrame, grayStored, cv::COLOR_BGR2GRAY);

    // Compute dense optical flow
    cv::Mat flow;
    cv::calcOpticalFlowFarneback(grayStored, grayTransformed, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

    // Extract the flow into X and Y channels
    std::vector<cv::Mat> flow_channels(2);
    cv::split(flow, flow_channels);
    cv::Mat flow_x = flow_channels[0];
    cv::Mat flow_y = flow_channels[1];

    // Generate remap grid
    cv::Mat map_x = cv::Mat::zeros(flow.size(), CV_32FC1);
    cv::Mat map_y = cv::Mat::zeros(flow.size(), CV_32FC1);
    for (int y = 0; y < flow.rows; ++y) 
    {
        for (int x = 0; x < flow.cols; ++x) 
        {
            map_x.at<float>(y, x) = x + flow_x.at<float>(y, x);
            map_y.at<float>(y, x) = y + flow_y.at<float>(y, x);
        }
    }

    // Warp the transformed frame using optical flow
    cv::Mat seamlessRegistered;
    cv::remap(transformedFrame, seamlessRegistered, map_x, map_y, cv::INTER_LINEAR);

    if (debug) 
    {
        cv::imshow("Seamless Registration: Stored vs Aligned Current", seamlessRegistered);
        cv::waitKey(0);
    }

    return seamlessRegistered;
}