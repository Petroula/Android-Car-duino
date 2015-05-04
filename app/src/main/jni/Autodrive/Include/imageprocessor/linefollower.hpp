#include "line.hpp"
#include "roadlinebuilder.hpp"
#include "lightnormalizer.hpp"

namespace Autodrive
{
    class linefollower
    {
        RoadLine roadLine;

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

        linefollower(const cv::Mat& cannied, POINT laneStartPoint, int center_x)
        {
            roadBuilder = make_unique<roadlinebuilder>(laneStartPoint, center_x);

            roadLine = roadBuilder->build(cannied, 25);

            targetRoadDistance = roadLine.getMeanStartDistance(5);
        }

        optional<int> getPreferedAngle()
        {
            float startDistance = targetRoadDistance;

            if (roadLine.points.size() > 5 && fabs(roadLine.getMeanAngle() - Direction::FORWARD) < Mathf::PI_2)
            {
                /* Start by setting the target angle to the mean road angle*/
                int degrees = Mathf::toDegrees(roadLine.getMeanAngle(4)) - 90;
                degrees = int((degrees / 46.f) * 25);
                degrees *= -1;
                // If the current distance is larger than, target distance, turn more right, vice versa
                startDistance = roadLine.getMeanStartDistance(5);
                float distanceDeviation = startDistance - targetRoadDistance;
                degrees += distanceDeviation;
                degrees = std::min(degrees, 25);
                degrees = std::max(degrees, -25);

                return degrees;
            }

            return nullptr;
        }

        void update(cv::Mat& cannied)
        {
            roadLine = roadBuilder->build(cannied, 17);
        }

    };

}
