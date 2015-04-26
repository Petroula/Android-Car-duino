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
    cv::Mat birds_eye_transform(cv::Mat& mat, cv::Mat birdseye_matrix)
    {
        cv::Mat warped_image;
        cv::warpPerspective(mat, warped_image, birdseye_matrix, mat.size(), cv::INTER_LINEAR);
        return warped_image;
    }

    roadfinder road;

    cv::Mat perspective;

    int thresh1 = 181;
    int thresh2 = 71;
    int intensity = 110;
    int blur_i = 11;

    cv::Point2f start_center;
    cv::Mat prev_mat;

    bool init_processing(cv::Mat mat)
    {
        cv::namedWindow("cannied_mat", 1);
        cv::createTrackbar("Thresh1:", "cannied_mat", &thresh1, 400);
        cv::createTrackbar("Thresh2:", "cannied_mat", &thresh2, 400);
        cv::createTrackbar("intensity:", "cannied_mat", &intensity, 300);
        cv::createTrackbar("blur_i:", "cannied_mat", &blur_i, 200);

        start_center = cv::Point2f(mat.size().width / 2.f , mat.size().height - 30.f);

        auto found_pespective = find_perspective(mat);
        if (found_pespective)
        {
            perspective = *found_pespective;
            cv::Mat warped_image = birds_eye_transform(mat, perspective);
            cv::Mat normalized_mat = normalizeLightning(warped_image, blur_i, intensity / 100.f);
            cv::Mat cannied_mat;
            cv::Canny(normalized_mat, cannied_mat, thresh1, thresh2, 3);
            prev_mat = normalized_mat;
            road.build(cannied_mat, start_center, 1);
            return true;
        } else
            return false;
    }



    command continue_processing(cv::Mat mat)
    {
        //while (true) {
        cv::Mat warped_image = birds_eye_transform(mat, perspective);
        cv::Mat normalized_mat = normalizeLightning(warped_image, blur_i, intensity / 100.f);

        //cv::Mat transform = cv::estimateRigidTransform(prev_mat, normalized_mat, true);

        //cv::perspectiveTransform(normalized_mat,normalized_mat, transform);
        
        cv::Mat cannied_mat;

        cv::Canny(normalized_mat, cannied_mat, thresh1, thresh2, 3);

        //auto testmatrix = cv::getPerspectiveTransform(prev_mat, cannied_mat);

        prev_mat = cannied_mat;

        road.update(cannied_mat,true,4.f);

        //cv::waitKey(10);} // waits to display frame

        //road.update(warped_image, 1);
        command cmd;
        cmd.setAngle(road.getAngle());
        return cmd;
    }
}