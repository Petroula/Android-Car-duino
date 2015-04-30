#pragma once
#include "line.hpp"
#include "util.hpp"

namespace Autodrive
{
    const float PI = Mathf::PI;
    const float PI_2 = Mathf::PI_2;

    struct lanes{
        linef left;
        linef right;
        bool found = false;
    };

    lanes getLaneMarkings(const cv::Mat& canniedMat,cv::Mat* drawMat){
        lanes lanes;
        ::std::vector<cv::Vec4i> lines;
        linef leftMostLine;
        linef rightMostline;
        cv::HoughLinesP(canniedMat, lines, 1, CV_PI / 180, 20, 10, 50);
        int leftmost = canniedMat.size().width;
        int rightmost = 0;
        bool foundLeft = false;
        bool foundRight = false;
        int center = canniedMat.size().width / 2;

        static linef lastLML;
        static linef lastRML;
        for(cv::Vec4i line : lines){
            int leftx = line[0];
            int rightx = line[2];
            int boty = line[1];
            int topy = line[3];
            linef vector(line);
            int thresh = 1;
            bool yes = false;


            float dirr = vector.direction_fixed_half();

            float dir_diff = dirr - PI_2;

            if (abs(dir_diff) < 0.f || abs(dir_diff) > 1.f)
                continue;


            if (leftx - vector.length() < leftmost && leftx > center + 20) {
                    if (rightx > leftx && topy > boty && vector.length() > 110) {
                        leftMostLine = linef(line);
                        foundLeft = true;
                    }
            }
            if (rightx + vector.length() > rightmost && rightx < center - 20)
            {
                if (rightx + vector.length() > rightmost && rightx < center - 20)
                {
                    rightMostline = linef(line);
                    foundRight = true;
                }
            }
        }


        lastLML = leftMostLine;
        lastRML = rightMostline;
        if (foundRight && foundLeft) {
            leftMostLine.draw(*drawMat,cv::Scalar(255,0,0),2);
            rightMostline.draw(*drawMat,cv::Scalar(255,0,0),2);
            if ( abs((-rightMostline.k) - leftMostLine.k) < 0.9f)
            {
                rightMostline.stretchY(0.f, (float) canniedMat.size().height);
                leftMostLine.stretchY(0.f, (float) canniedMat.size().height );
                //TODO: Deprecated line
                //if ((leftMostLine.leftMost_x() >rightMostline.rightMost_x()))
                {
                    leftMostLine.draw(*drawMat, cv::Scalar(0, 0, 255), 5);
                    rightMostline.draw(*drawMat,cv::Scalar(0,0,255),5);
                    lanes.left = rightMostline;
                    lanes.right = leftMostLine;
                    float dirLeft = lanes.left.k;
                    float dirRight = lanes.right.k;
                    lanes.found = true;
                }
            }
        }

        return lanes;
    }

    linef leftImageBorder;
    linef rightImageBorder;
    float centerDiff;

    optional<cv::Mat> find_perspective(cv::Mat* matIn, double thresh1 = 300, double thresh2 = 150, int crop = 5){
        optional<cv::Mat> birdseye_matrix;
        cv::Mat matCopy = matIn->clone();

        cv::erode(matCopy, matCopy, cv::Mat(), cv::Point(-1, -1), 1);
        cv::Mat cannied;
        cv::Canny(matCopy, cannied, thresh1, thresh2, 3);
        matCopy = cannied;
        auto lines = getLaneMarkings(matCopy,matIn);
        if (!lines.found)
            return nullptr;
            
        linef leftLine = lines.left;
        linef rightLine = lines.right;

        float icrop = 0.f;
        float xdiff;
        float height = (float) matCopy.size().height;
        float width = (float) matCopy.size().width;
        do
        {
            xdiff = rightLine.leftMost_x() - leftLine.rightMost_x();
            rightLine.stretchY(icrop, height);
            leftLine.stretchY(icrop, height);
            icrop+=3.f;
        } while (xdiff < width/3.0f);

        float right = width;
        float bottom = height;
        float xleft = leftLine.end.x;
        float xright = rightLine.end.x;
        bool warping = true;
//#define _VISUAL_WARP
#ifdef _VISUAL_WARP
        while (warping){
            if (xleft < leftLine.begin.x || xright > rightLine.begin.x)
            {
                if (xleft < leftLine.begin.x){
                    xleft++;
                }
                if (xright > rightLine.begin.x){
                    xright--;
                }
            }
            else
#endif
            {
                warping = false;
                xright = rightLine.begin.x;
                xleft = leftLine.begin.x;
            }
            
            centerDiff = abs(xleft + xright) / 2.f - width /2.f;

            //Crop moves the two upper cordinates farther appart, both from each other and from the lower cordinates (Outside the image)
            cv::Point2f pts1[] = { leftLine.begin, rightLine.begin, cv::Point2f(leftLine.end.x, bottom), cv::Point2f(rightLine.end.x, bottom) };

            //Warp compresses the bottom two cordinates together
            cv::Point2f pts2[] = { leftLine.begin, rightLine.begin, cv::Point2f(xleft, bottom), cv::Point2f(xright, bottom) };

            birdseye_matrix = cv::getPerspectiveTransform(pts1, pts2);

            leftImageBorder = linef(cv::Point2f(xleft - leftLine.end.x / 2, leftLine.end.y +2), cv::Point2f(0, leftLine.begin.y+2));
            rightImageBorder = linef(cv::Point2f(xright - (rightLine.end.x - width)/2, rightLine.end.y+2), cv::Point2f(width, rightLine.begin.y+2));
            
#ifdef _VISUAL_WARP
            cv::Mat warped_image;
            cv::warpPerspective(matCopy, warped_image, *birdseye_matrix, matCopy.size(), cv::INTER_LINEAR);

            cv::resize(warped_image, warped_image, warped_image.size() * 3);//resize image
            cv::imshow("mainwindow", warped_image);
            cv::waitKey(1); // waits to display frame
        }
#endif
        return birdseye_matrix;
    }

    void birds_eye_transform(cv::Mat* mat, cv::Mat birdseye_matrix)
    {
        cv::warpPerspective(*mat, *mat, birdseye_matrix, mat->size(), cv::INTER_LINEAR);
    }
}
