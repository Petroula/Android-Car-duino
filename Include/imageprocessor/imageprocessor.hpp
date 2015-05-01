#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "command.hpp"
#include "util.hpp"
#include "roadfinder.hpp"
#include "lightnormalizer.hpp"

using namespace std;

int intersection_protect = 0;

#define _AUTODRIVE_DILATE

#define _DEBUG

#include "birdseyetransformer.hpp"

namespace Autodrive
{
    namespace imageProcessor
    {
        roadfinder road;

        cv::Mat perspective;

        int thresh1 = 181;
        int thresh2 = 71;
        int intensity = 110;
        int blur_i = 11;

        cv::Point2f start_center;

        bool init_processing(cv::Mat* mat)
        {
            auto found_pespective = find_perspective(mat);
            if (found_pespective)
            {
                perspective = *found_pespective;
                birds_eye_transform(mat, perspective);
                start_center = cv::Point2f(mat->size().width / 2.f + centerDiff, mat->size().height - 35.f);
                normalizeLightning(mat, blur_i, intensity / 100.f);
                cv::Mat cannied_mat;
                cv::Canny(*mat, cannied_mat, thresh1, thresh2, 3);
                road.build2(cannied_mat, start_center);
                return true;
            } else{
                cv::putText(*mat, "SEARCHING FOR STRAIGHT LANES...", cv::Point2f(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0), 2);
                return false;
            }
        }

        command continue_processing(cv::Mat& mat)
        {
            birds_eye_transform(&mat, perspective);
            normalizeLightning(&mat, blur_i, intensity / 100.f);

            cv::Mat cannied_mat;
            cv::Canny(mat, cannied_mat, thresh1, thresh2, 3);

            /* PAINT OVER BORDER ARTEFACTS FROM TRANSFORM*/
            leftImageBorder.draw(cannied_mat, cv::Scalar(0, 0, 0), 10);
            rightImageBorder.draw(cannied_mat, cv::Scalar(0, 0, 0), 10);
            
            command cmnd = road.update2(cannied_mat,mat);
            float angle = Autodrive::Mathf::PI_2;

            if (cmnd.changedAngle)
            {
                angle = ((90 - cmnd.angle*1.5)* Autodrive::Mathf::PI) / 180.f ;
            }

            //leftImageBorder.draw(mat, cv::Scalar(0, 255, 255), 8);
            //rightImageBorder.draw(mat, cv::Scalar(0, 255, 255), 8);
            
            cv::Point2f center(mat.size().width / 2.f, (float) mat.size().height);
            Autodrive::linef(center, center + cv::Point2f(std::cos(angle) * 200, -sin(angle) * 200)).draw(mat, CV_RGB(0, 250, 0));
            return cmnd;
        }
    }
}
