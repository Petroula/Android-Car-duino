#include <vector>
#include "line.hpp"
#include "util.hpp"
#include <numeric>
#include "../settings.hpp"

namespace Autodrive
{
    struct RoadLine
    {
        std::vector<POINT> points;
        std::vector<int> distances;
        std::vector<float> angles;
        std::vector<float> angleDiffs;
        int totalGap = 0;

        int centerX;

        void draw(cv::Mat* drawMat)
        {
            for (unsigned int i = 0; i < points.size() - 1; i++)
            {
                linef(points[i], points[i + 1]).draw(*drawMat, cv::Scalar(255, 0, 0), 4);
            }
        }

        bool addPoint(POINT p)
        {
            float prevAngle = angles.back();
            float newAngle = Autodrive::linef(points.back(), p).direction_fixed_half();

            float mean = getMeanAngle(0);
            //Almost never happens - ?
            if (fabs(newAngle - mean) > Settings::maxAngleDiff)//Mathf::PI_2 / 2.0f)
                return false;

            angles.push_back(newAngle);
            angleDiffs.push_back(newAngle - prevAngle);
            points.push_back(p);
            distances.push_back(p.x - centerX);

            return true;
        }

        RoadLine(int center_x, POINT start_point) : centerX(center_x)
        {
            angles.push_back(Direction::FORWARD);
            points.push_back(start_point);
            angleDiffs.push_back(0);
        }

        RoadLine()
        {}

        float getMeanAngle(unsigned int lastSize = 0)
        {
            unsigned int offset = 0;
            if (lastSize > 0 && angles.size() > lastSize)
                offset = angles.size() - lastSize;
            return std::accumulate(angles.begin() + offset, angles.end(), 0.f) / (float) (angles.size() - offset);
        }

        float getMeanAngleDiffs(unsigned int lastSize = 0)
        {
            unsigned int offset = 0;
            if (lastSize > 0 && angles.size() > lastSize)
                offset = angles.size() - lastSize;
            return std::accumulate(angleDiffs.begin() + offset, angleDiffs.end(), 0.f) / ((float) angleDiffs.size() - offset);
        }

        float getEstimatedAngle(int n = 20)
        {
            return angles.back() + getMeanAngleDiffs(n);
        }

        float getMeanStartDistance(unsigned int nDistancesFromBegin)
        {
            if (distances.size() == 0)
                return 0;
                
            if (nDistancesFromBegin >= distances.size())
                nDistancesFromBegin = int(distances.size());
            return std::accumulate(distances.begin(), distances.begin() + nDistancesFromBegin, 0.f) / float(nDistancesFromBegin);
        }
    };
}
