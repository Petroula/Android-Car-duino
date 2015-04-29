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

    lanes getLaneMarkings(cv::Mat canniedMat, bool reset = false){
        lanes lanes;
        ::std::vector<cv::Vec4i> lines;
        linef leftMostLine;
        linef rightMostline;
        cv::HoughLinesP(canniedMat, lines, 1, CV_PI / 180, 20, 10, 50);
        int leftmost = canniedMat.size().width;
        int rightmost = 0;
        static bool first = true;
        bool foundLeft = false;
        bool foundRight = false;
        int center = canniedMat.size().width / 2;

        if (reset) first = true;
//#define DEBUG_LANE_MARKINGS
#ifdef DEBUG_LANE_MARKINGS
        cv::Mat colorCopy;
        cv::cvtColor(canniedMat, colorCopy, CV_GRAY2RGB);
#endif 

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

            if (leftx - vector.length() < leftmost && leftx > center + 40)
            {
                if (first || vector.differs_less_than_from(lastLML, 500, 0.6f)){
                    if (rightx > leftx && topy > boty && vector.length() > 110)
                    {
                        leftMostLine = linef(line);
                        foundLeft = true;
                    }
                }
            }

            if (rightx + vector.length() > rightmost && rightx < center - 40)
            {
                if (first || vector.differs_less_than_from(lastRML, 1600 * 100, 5.f)){
                    rightMostline = linef(line);
                    foundRight = true;
                }
            }
        }


#ifdef DEBUG_LANE_MARKINGS
        linef lineleft(cv::Point2f(center, 40), cv::Point2f(center, canniedMat.size().height));
        lineleft.draw(colorCopy);
        float dir = lineleft.direction();
        leftMostLine.draw(colorCopy, cv::Scalar(0, 0, 255), 5);
        rightMostline.draw(colorCopy,cv::Scalar(0,0,255),5);
        cv::resize(colorCopy, colorCopy, colorCopy.size() * 2);//resize image
        cv::namedWindow("Lane_Markings", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
        cv::imshow("Lane_Markings", colorCopy);
#endif


        lastLML = leftMostLine;
        lastRML = rightMostline;

        if (foundRight && foundLeft) {
            if ( abs((-rightMostline.k) - leftMostLine.k) < 0.8f)
            {
                rightMostline.stretchY(0.f, (float) canniedMat.size().height);
                leftMostLine.stretchY(0.f, (float) canniedMat.size().height );

                if ((leftMostLine.leftMost_x() >rightMostline.rightMost_x()))
                {
                    lanes.left = rightMostline;
                    lanes.right = leftMostLine;
                    float dirLeft = lanes.left.k;
                    float dirRight = lanes.right.k;
                    lanes.found = true;
                    first = false;
                }
            }
        }

        return lanes;
    }

    linef leftImageBorder;
    linef rightImageBorder;
    float centerDiff;

    optional<cv::Mat> find_perspective(cv::Mat matIn, double thresh1 = 300, double thresh2 = 150, int crop = 5){
        optional<cv::Mat> birdseye_matrix;
        cv::Mat mat = matIn.clone();
        cv::erode(mat, mat, cv::Mat(), cv::Point(-1, -1), 1);
        //cv::Mat cannied = canny(mat);
        cv::Mat cannied;
        cv::Canny(mat, cannied, thresh1, thresh2, 3);

        auto lines = getLaneMarkings(cannied);
        if (!lines.found)
            return nullptr;

        linef leftLine = lines.left;
        linef rightLine = lines.right;

        float icrop = 0.f;
        float xdiff;
        do
        {
            xdiff = rightLine.leftMost_x() - leftLine.rightMost_x();
            rightLine.stretchY(icrop, (float) mat.size().height);
            leftLine.stretchY(icrop, (float) mat.size().height);
            icrop+=2.f;
        } while (xdiff < mat.size().width/5.f);

        rightLine.draw(mat, cv::Scalar(0, 255, 0), 4);
        leftLine.draw(mat, cv::Scalar(0, 255, 0), 4);

        float right = (float)mat.size().width;
        float bottom = (float)mat.size().height;

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
            
            centerDiff = abs(xleft + xright) / 2.f - mat.size().width /2.f;

            //Crop moves the two upper cordinates farther appart, both from each other and from the lower cordinates (Outside the image)
            cv::Point2f pts1[] = { leftLine.begin, rightLine.begin, cv::Point2f(leftLine.end.x, bottom), cv::Point2f(rightLine.end.x, bottom) };

            //Warp compresses the bottom two cordinates together
            cv::Point2f pts2[] = { leftLine.begin, rightLine.begin, cv::Point2f(xleft, bottom), cv::Point2f(xright, bottom) };

            birdseye_matrix = cv::getPerspectiveTransform(pts1, pts2);

            leftImageBorder = linef(cv::Point2f(leftLine.begin.x - centerDiff, leftLine.end.y -4), cv::Point2f(0, leftLine.begin.y -4));
            rightImageBorder = linef(cv::Point2f(rightLine.begin.x - centerDiff, rightLine.end.y-4), cv::Point2f(mat.size().width, rightLine.begin.y-4));

#ifdef _VISUAL_WARP
            cv::Mat warped_image;
            cv::warpPerspective(mat, warped_image, *birdseye_matrix, mat.size(), cv::INTER_LINEAR);

            cv::resize(warped_image, warped_image, warped_image.size() * 3);//resize image
            cv::imshow("mainwindow", warped_image);
            cv::waitKey(1); // waits to display frame
        }
#endif
        return birdseye_matrix;
    }

    cv::Mat birds_eye_transform(cv::Mat& mat, cv::Mat birdseye_matrix)
    {
        cv::Mat warped_image;
        cv::warpPerspective(mat, warped_image, birdseye_matrix, mat.size(), cv::INTER_LINEAR);
        return warped_image;
    }
}
