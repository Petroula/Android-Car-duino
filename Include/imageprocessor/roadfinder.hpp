#include "line.hpp"
#include <queue>
#include "roadbuilder.hpp"
#include <numeric>
#include "lightnormalizer.hpp"
#include <memory>

namespace Autodrive
{
    class roadfinder
    {
        typedef cv::Point_ < float > POINT;
        roadbuilder::Road road;

    public:

        POINT start_center;
        std::unique_ptr<roadbuilder> roadBuilder = nullptr;

        cv::Mat draw2(const cv::Mat& cannied)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            for (unsigned int i = 0; i < road.points.size() -1; i++)
            {
                linef(road.points[i], road.points[i + 1]).draw(colorCopy,cv::Scalar(255,0,0),1);
            }

            linef(roadBuilder->last_start, roadBuilder->last_start + POINT(8, -15)).draw(colorCopy, cv::Scalar(0, 255, 255), 1);
            linef(start_center, POINT(start_center.x, 0)).draw(colorCopy);
            
            return colorCopy;
        }

        void build2(const cv::Mat& cannied, POINT start_point, int _updateFrequency) 
        {
            roadBuilder = make_unique<roadbuilder>(cannied, start_point);
            road = roadBuilder->build2(cannied, 25);
        }

        command update2(cv::Mat& cannied,cv::Mat& drawMat)
        {
            road = roadBuilder->build2(cannied, 25);
            
            drawMat = draw2(cannied);
            command cmd;

            int mid = int(road.points.size() / 2.5);
            
            if (road.points.size() < 5 || abs(road.getMeanAngle() - Mathf::PI_2) > Mathf::PI_2)
                cmd.setSpeed(60);
            else
            {
                int degrees = Mathf::toDegrees(road.getMeanAngle()) - 90;
                degrees = int((degrees / 50.f) * 25);

                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);


                cmd.setAngle(degrees*-1);
            }

            return cmd;
        }

    };

}
