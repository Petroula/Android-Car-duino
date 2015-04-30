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
        float targetRoadDistance = 0;
        std::unique_ptr<roadbuilder> roadBuilder = nullptr;

        cv::Mat draw2(const cv::Mat& cannied)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            for (unsigned int i = 0; i < road.points.size() -1; i++)
            {
                linef(road.points[i], road.points[i + 1]).draw(colorCopy,cv::Scalar(255,0,0),4);
            }

            linef(roadBuilder->last_start, roadBuilder->last_start + POINT(8, -20)).draw(colorCopy, cv::Scalar(0, 255, 255), 1);
            //linef(start_center, POINT(start_center.x, 0)).draw(colorCopy);
            
            return colorCopy;
        }

        void build2(const cv::Mat& cannied, POINT start_point, int _updateFrequency) 
        {
            roadBuilder = make_unique<roadbuilder>(cannied, start_point);
            road = roadBuilder->build2(cannied, 25);
            targetRoadDistance = road.getMeanStartDistance(5);
        }

        command update2(cv::Mat& cannied,cv::Mat& drawMat)
        {
            road = roadBuilder->build2(cannied, 17);
            
            drawMat = draw2(cannied);
            command cmd;

            int mid = int(road.points.size() / 2.5);
            
            if (road.points.size() > 5 && abs(road.getMeanAngle() - Mathf::PI_2) < Mathf::PI_2)
            {
                /* Start by setting the target angle to the mean road angle*/
                int degrees = Mathf::toDegrees(road.getMeanAngle(4)) - 90;
                degrees = int((degrees / 46.f) * 25);
                degrees*= -1;
                /* Add the difference of the distance to the parallel road and the targetDistance*/
                degrees += std::pow((targetRoadDistance - road.getMeanStartDistance(5))*5.f,1.8f);
                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);

                cmd.setAngle(degrees);
            }

            return cmd;
        }

    };

}
