#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

namespace Autodrive
{
    void normalizeLightning(cv::Mat* bgr_image,int blur = 20,float intensity = 0.5f)
    {
        cv::Mat light_mat;
        cv::blur(*bgr_image, light_mat, cv::Size(blur, blur));
        cv::cvtColor(light_mat, light_mat, CV_BGR2GRAY);

        cv::Mat lab_image;
        cv::cvtColor(*bgr_image, lab_image, CV_BGR2Lab);

        // Extract the L channel
        std::vector<cv::Mat> lab_planes(3);
        cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

        lab_planes[0] = lab_planes[0] - light_mat*intensity;

        // Merge the the color planes back into an Lab image
        cv::merge(lab_planes, lab_image);

        // convert back to RGB
        cv::cvtColor(lab_image, *bgr_image, CV_Lab2BGR);
    }
}
