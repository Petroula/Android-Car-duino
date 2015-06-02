#include "line.hpp"
#include "roadlinebuilder.hpp"
#include "lightnormalizer.hpp"
#include <chrono>

namespace Autodrive
{
    class linefollower
    {
        RoadLine roadLine;
        int roadsize = 40;
        bool isfound = false;
    public:
        float targetRoadDistance = 0;
        std::unique_ptr<roadlinebuilder> roadBuilder = nullptr;

        void draw(cv::Mat* colorCopy, int centerX)
        {
            roadLine.draw(colorCopy);
           
            /* DRAW RECTANGLE FOR POSSIBLE FIRST HITS*/
            POINT upperLeft = roadBuilder->last_start + POINT(-Settings::leftIterationLength, Settings::firstFragmentMaxDist);
            POINT lowerRight = roadBuilder->last_start + POINT(Settings::rightIterationLength, 0);
            cv::rectangle(*colorCopy,upperLeft , lowerRight,cv::Scalar(255,0,255));
            
            //linef(roadBuilder->last_start, roadBuilder->last_start + POINT(8, -20)).draw(colorCopy, cv::Scalar(0, 255, 255), 1);

            /* DRAW VERTICAL LINE DISPLAYING DISTANCE TO ROAD AND TARGETED DISTANCE TO ROAD*/
            POINT offsetX = POINT(targetRoadDistance,0);
            POINT bottom_center = POINT(centerX, colorCopy->size().height);
            linef(bottom_center + offsetX, POINT(centerX, 0) + offsetX).draw(*colorCopy);
            offsetX.x = roadLine.getMeanStartDistance(5);
            if (int(offsetX.x) != 0)
                linef(bottom_center + offsetX, POINT(centerX, 0) + offsetX).draw(*colorCopy, cv::Scalar(255, 255, 0));
        }

        linefollower(const cv::Mat& cannied, POINT laneStartPoint, int center_x,int carY)
        {
            roadBuilder = make_unique<roadlinebuilder>(laneStartPoint, center_x,carY);

            roadLine = roadBuilder->build(cannied, roadsize);

            targetRoadDistance = roadLine.getMeanStartDistance(5);
        }


        // Prerequicite for wheter a road is found or not
        bool isFound()
        {
            return isfound;
        }

        float distanceDeviation()
        {
            if(!isFound())
                return targetRoadDistance;
            float startDistance = roadLine.getMeanStartDistance(4);
            return (startDistance - targetRoadDistance) * 1.1f;
        }

        int totalGap()
        {
            return roadLine.totalGap / roadLine.points.size();
        }

        optional<int> getPreferedAngle()
        {
            if (isFound())
            {
                /* Start by setting the target angle to the mean road angle*/
                int degrees = Mathf::toDegrees(roadLine.getMeanAngle(4)) - 90;
                degrees = int((degrees / 65.f) * 25);
                degrees *= -1;
                
                degrees+=distanceDeviation();
                
                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);
                return degrees;
            }

            return nullptr;
        }

        void update(cv::Mat& cannied)
        {
            roadLine = roadBuilder->build(cannied, roadsize);
            isfound = (roadLine.points.size() > 5 && fabs(roadLine.getMeanAngle() - Direction::FORWARD) < Mathf::PI_2);
        }

    };

}

