#include "line.hpp"
#include <queue>
#include "roadfragmentbuilder2.hpp"
#include <numeric>
#include "lightnormalizer.hpp"
#include <memory>

namespace Autodrive
{
    class roadfinder
    {
        typedef cv::Point_ < float > POINT;
        const float PI = Mathf::PI;
        const float PI_2 = Mathf::PI_2;

        roadfragmentbuilder2::Road road2;

        int canny_thresh1;
        int canny_thresh2;

    public:

        int updateFrequency;
        int updateCounter;

        float getWay(float f)
        {
            if (f < 0.f)
                return -1;
            if (f == 0.f)
                return 0;
            if (f > 0.f)
                return 1;
            throw;
        }

        POINT start_center;
        std::unique_ptr<roadfragmentbuilder2> roadbuilder2 = nullptr;

        cv::Mat draw2(const cv::Mat& cannied)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            for (unsigned int i = 0; i < road2.points.size() -1; i++)
            {
                linef(road2.points[i], road2.points[i + 1]).draw(colorCopy,cv::Scalar(255,0,0),1);
            }

            linef(roadbuilder2->last_start, roadbuilder2->last_start + POINT(8,-15)).draw(colorCopy,cv::Scalar(0,255,255),1);
            linef(start_center, POINT(start_center.x, 0)).draw(colorCopy);
            
            return colorCopy;
            /*cv::resize(colorCopy, colorCopy, colorCopy.size() * 3);//resize image
            cv::namedWindow("mainwindow", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
            cv::imshow("mainwindow", colorCopy);
            cv::waitKey(10); // waits to display frame*/
        }

        void build2(const cv::Mat& cannied, POINT start_point, int _updateFrequency)
        {
            updateFrequency = _updateFrequency;
            roadbuilder2 = make_unique<roadfragmentbuilder2>(cannied, start_point);
        }

        command update2(cv::Mat& cannied,cv::Mat& drawMat)
        {
            road2 = roadbuilder2->build2(cannied, 50);
            
            drawMat = draw2(cannied);
            command cmd;

            int mid = int(road2.points.size() / 2.5);
            
            if (road2.points.size() < 5 || abs(road2.getMeanAngle() - Mathf::PI_2) > Mathf::PI_2)
                cmd.setSpeed(60);
            else
            {
                int degrees = Mathf::toDegrees(road2.getMeanAngle()) - 90;
                degrees = int((degrees / 50.f) * 25);

                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);


                cmd.setAngle(degrees*-1);
            }

            return cmd;
        }

    };

}
