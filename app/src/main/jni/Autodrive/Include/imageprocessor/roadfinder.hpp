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

        cv::Mat draw2(const cv::Mat& cannied,float startDist)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            for (unsigned int i = 0; i < road.points.size() -1; i++)
            {
                linef(road.points[i], road.points[i + 1]).draw(colorCopy,cv::Scalar(255,0,0),4);
            }

            linef(roadBuilder->last_start, roadBuilder->last_start + POINT(8, -20)).draw(colorCopy, cv::Scalar(0, 255, 255), 1);


            /* DRAW VERTICAL LINE DISPLAYING DISTANCE TO ROAD AND TARGETED DISTANCE TO ROAD*/
            POINT offsetX = POINT(targetRoadDistance,0);
            linef(roadBuilder->center + offsetX, POINT(roadBuilder->center.x,0) + offsetX).draw(colorCopy);
            offsetX.x = startDist;
            linef(roadBuilder->center + offsetX, POINT(roadBuilder->center.x,0) + offsetX).draw(colorCopy,cv::Scalar(255,255,0));

            return colorCopy;
        }

        void build2(const cv::Mat& cannied, POINT start_point)
        {
            roadBuilder = make_unique<roadbuilder>(cannied, start_point);
            road = roadBuilder->build2(cannied, 25);
            targetRoadDistance = road.getMeanStartDistance(5);
        }

        command update2(cv::Mat& cannied,cv::Mat& drawMat)
        {
            road = roadBuilder->build2(cannied, 17);

            command cmd;
            float startDistance = targetRoadDistance;

            if (road.points.size() > 5 && abs(road.getMeanAngle() - Mathf::PI_2) < Mathf::PI_2)
            {
                /* Start by setting the target angle to the mean road angle*/
                int degrees = Mathf::toDegrees(road.getMeanAngle(4)) - 90;
                degrees = int((degrees / 46.f) * 25);
                degrees*= -1;
                // If the current distance is larger than, target distance, turn right more right, vice versa
                startDistance = road.getMeanStartDistance(5);
                float distanceDeviation = startDistance  - targetRoadDistance ;
                degrees += distanceDeviation;
                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);

                cmd.setAngle(degrees);
            }

            drawMat = draw2(cannied,startDistance);

            return cmd;
        }

    };

}
