#include "line.hpp"
#include "roadlinebuilder.hpp"
#include "lightnormalizer.hpp"
#include <unistd.h>
#include <chrono>

namespace Autodrive
{
    class linefollower
    {
        optional<RoadLine> roadLine;
        int roadsize = 28;
    public:
        float targetRoadDistance = 0;
        std::unique_ptr<roadlinebuilder> roadBuilder = nullptr;

        void draw(cv::Mat* colorCopy, int centerX)
        {
            if(roadLine == nullptr)
                return;
            roadLine->draw(colorCopy);
           
            /* DRAW RECTANGLE FOR POSSIBLE FIRST HITS*/
            POINT upperLeft = roadBuilder->last_start + POINT(-Settings::leftIterationLength, Settings::firstFragmentMaxDist);
            POINT lowerRight = roadBuilder->last_start + POINT(Settings::rightIterationLength, 0);
            cv::rectangle(*colorCopy,upperLeft , lowerRight,cv::Scalar(255,0,255));

            /* DRAW VERTICAL LINE DISPLAYING DISTANCE TO ROAD AND TARGETED DISTANCE TO ROAD*/
            POINT offsetX = POINT(targetRoadDistance,0);
            POINT bottom_center = POINT(centerX, colorCopy->size().height);
            linef(bottom_center + offsetX, POINT(centerX, 0) + offsetX).draw(*colorCopy);
            offsetX.x = roadLine->getMeanStartDistance(5);
            if (int(offsetX.x) != 0)
                linef(bottom_center + offsetX, POINT(centerX, 0) + offsetX).draw(*colorCopy, cv::Scalar(255, 255, 0));
        }

        linefollower(const cv::Mat& cannied, POINT laneStartPoint, int center_x,float angle_in,float angle_out)
        {
            roadBuilder = make_unique<roadlinebuilder>(laneStartPoint, center_x,angle_in,angle_out);
            roadLine = roadBuilder->build(cannied, roadsize);
            if(!roadLine.valid)
                throw "Could not detect start of line";
            targetRoadDistance = roadLine->getMeanStartDistance(5);
        }

        float distanceDeviation()
        {
            if(!roadLine)
                return targetRoadDistance;
            float startDistance = roadLine->getMeanStartDistance(3);
            return (startDistance - targetRoadDistance) * 1.3f;

        }

        optional<int> getPreferedAngle()
        {
            if (roadLine)
            {
                /* Start by setting the target angle to the mean road angle*/
                int degrees = Mathf::toDegrees(roadLine->getMeanAngle(4) - Mathf::PI_2) ;
                degrees = int((degrees / 48.2f) * 24);
                degrees *= -1;
                // If the current distance is larger than, target distance, turn more right, vice versa
                degrees += distanceDeviation();
                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);

                return degrees;
            }

            return nullptr;
        }

        void update(cv::Mat& cannied)
        {
            roadLine = roadBuilder->build(cannied, roadsize);
        }

    };

}

