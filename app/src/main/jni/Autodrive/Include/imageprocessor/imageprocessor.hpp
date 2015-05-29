#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "../command.hpp"
#include "util.hpp"
#include "lightnormalizer.hpp"
#include "../settings.hpp"
#include "roadfollower.hpp"

using namespace std;

int intersection_protect = 0;

#define _AUTODRIVE_DILATE

#define _DEBUG

#include "birdseyetransformer.hpp"

namespace Autodrive
{
    namespace imageProcessor
    {
        unique_ptr<roadfollower> roadFollower = nullptr;

        cv::Mat perspective;

        int thresh1 = 181;
        int thresh2 = 71;
        int intensity = 110;
        int blur_i = 11;

        POINT start_center;

        bool init_processing(cv::Mat* mat)
        {
            auto found_pespective = find_perspective(mat);
            if (found_pespective)
            {
                perspective = *found_pespective;
                birds_eye_transform(mat, perspective);
                if (Settings::normalizeLightning)
                    normalizeLightning(mat, blur_i, intensity / 100.f);
                cv::Mat cannied_mat;
                cv::Canny(*mat, cannied_mat, thresh1, thresh2, 3);
                roadFollower = make_unique<roadfollower>(cannied_mat, mat->size().width / 2.f + centerDiff);
                return true;
            } else{
                cv::putText(*mat, "SEARCHING FOR STRAIGHT LANES...", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                return false;
            }
        }

        command continue_processing(cv::Mat& mat)
        {
            birds_eye_transform(&mat, perspective);

            if (Settings::normalizeLightning)
                normalizeLightning(&mat, blur_i, intensity / 100.f);

            cv::Mat cannied_mat;
            cv::Canny(mat, cannied_mat, thresh1, thresh2, 3);

            /* PAINT OVER BORDER ARTEFACTS FROM TRANSFORM*/
            leftImageBorder.draw(cannied_mat, cv::Scalar(0, 0, 0), Settings::transformLineRemovalThreshold);
            rightImageBorder.draw(cannied_mat, cv::Scalar(0, 0, 0), Settings::transformLineRemovalThreshold);
            
            command cmnd = roadFollower->update(cannied_mat, mat);
            float angle =  Direction::FORWARD;

            if (cmnd.changedAngle)
            {
                //TODO: *15 really needed
                angle = ((90 - cmnd.angle*15)* Autodrive::Mathf::PI) / 180.f ;
            }

            //leftImageBorder.draw(mat, cv::Scalar(0, 255, 255), 8);
            //rightImageBorder.draw(mat, cv::Scalar(0, 255, 255), 8);
            
            POINT center(mat.size().width / 2.f, (float) mat.size().height);
            Autodrive::linef(center, center + POINT(std::cos(angle) * 200, -sin(angle) * 200)).draw(mat, CV_RGB(0, 250, 0));
            return cmnd;
        }

        bool leftLineFound()
        {
            return roadFollower->leftLineFound();
        }

        bool rightLineFound()
        {
            return roadFollower->rightLineFound();
        }

        /*
            Returns wether the car is on the left lane
            Currently only works if both roadlines are found by comparing their gaps
        */
        optional<bool> isLeftLane()
        {
            if (!leftLineFound() || !rightLineFound())
                return nullptr;
            else
                return roadFollower->isLeftLane();
        }

        /*
        Returns wether the car is on the right lane
        */
        optional<bool> isRightLane()
        {
            optional<bool> isLeft = isLeftLane();
            if (isLeft.valid)
                return !(*isLeft);
            else
                return nullptr;
        }
    }
}
