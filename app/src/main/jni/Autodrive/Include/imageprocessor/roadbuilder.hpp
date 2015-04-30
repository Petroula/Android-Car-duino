#pragma once
#include "line.hpp"
#include "util.hpp"
#include <numeric>

namespace Autodrive
{
    typedef cv::Point_ < float > POINT;

    class roadbuilder
    {
        const int distanceToRoad = 20;

        POINT getFirstHit(const cv::Mat& cannied, POINT start_point)
        {
            SearchResult right;
            int unfound = 0;

            //SEARCH UPWARDS UNTIL HIT
            while (!right.found && unfound++ < 9)
            {
                right = firstnonzero_direction(cannied, start_point, 0, 360);// 0 = RIGHT
                if (!right.found)
                    start_point.y--;
            }

            return right.point;
        }


        static SearchResult findFragment2(const cv::Mat& cannied, POINT start, float leftAngle, float rightAngle)
        {
            SearchResult rightSearch = firstnonzero_direction(cannied, start, rightAngle, 10);
            SearchResult leftSearch = firstnonzero_direction(cannied, start, leftAngle, 9);
            bool left = false;
            bool right = false;
            if (leftSearch.found && rightSearch.found)
            {
                if (leftSearch.distance <= rightSearch.distance + 3)
                {
                    {
                        return leftSearch;
                    }
                } else
                {
                    return rightSearch;
                }
            } else if (leftSearch.found)
            {
                return leftSearch;
            }

            return rightSearch;
        }


        POINT getFirstFragment2(const cv::Mat& cannied)
        {
            SearchResult searchRes;
            int unfound = 0;

            float right = 0.f;
            float up = Mathf::PI_2;
            float left = Mathf::PI;
            float down = Mathf::PI + Mathf::PI_2;

            POINT start_point = last_start;

            //SEARCH UPWARDS UNTIL HIT
            while (!searchRes.found && unfound++ < 20)
            {
                searchRes = findFragment2(cannied, start_point, left, right);
                if (!searchRes.found)
                    start_point.y--;
            }

            // SEARCH DOWNWARDS IF HIT ON FIRST Y AXIS
            SearchResult new_hit;
            new_hit = searchRes;
            int yStart = 0;
            while (new_hit.found && unfound == 1 && yStart++ < 5 && searchRes.point.y < cannied.size().height - 10)
            {
                new_hit.point.y++;
                new_hit = findFragment2(cannied, new_hit.point, left, right);
                if (new_hit.found && new_hit.point.x == searchRes.point.x)
                {
                    searchRes = new_hit;
                } else
                    break;
            }

            if (linef(first_start, searchRes.point).length2() <= 50*50)
                last_start = searchRes.point;

            return searchRes.point;
        }


        static optional<POINT> getNextPoint(const cv::Mat& cannied, float est_angle, const POINT& prevPoint,int delta = 2)
        {

            POINT it = prevPoint;
            bool vertical_search = false;
            float left_dir = Mathf::PI;
            float right_dir = 0;
            SearchResult searchResult;
            int unfound = 0;
            while (!searchResult.found && unfound < 10)
            {
                it.y-=delta;
                it.x += cosf(est_angle)*delta;
                searchResult = findFragment2(cannied, it, left_dir, right_dir);
                
                unfound++;
            }

            if (searchResult.found)
                return searchResult.point;
            else
                return nullptr;
        }

    public:
        POINT center;
        POINT last_start;
        POINT first_start;

        roadbuilder(const cv::Mat& cannied, POINT start_center) : center(start_center)
        {
            first_start = getFirstHit(cannied, start_center);
            last_start = first_start;
        }

        //TODO: refactor getter for mean angle etc
        struct Road
        {
            std::vector<POINT> points;
            std::vector<int> distances;
            std::vector<float> angles;
            std::vector<float> angleDiffs;

            int centerX;

            bool addPoint(POINT p)
            {
                float prevAngle = angles.back();
                float newAngle = linef(points.back(), p).direction_fixed_half();

                float mean = getMeanAngle(20);
                //Almost never happens - ?
                if (abs(newAngle - mean) > Mathf::PI_2 / 1.4f)
                    return false;

                angles.push_back(newAngle);
                angleDiffs.push_back(prevAngle - newAngle);
                points.push_back(p);
                distances.push_back(p.x - centerX);

                return true;
            }

            Road(int center_x,POINT start_point) : centerX(center_x)
            {
                angles.push_back(Mathf::PI_2);
                points.push_back(start_point);
                angleDiffs.push_back(0);
            }

            Road()
            {
            }

            float getMeanAngle(int lastSize = 0)
            {
                int offset = 0;
                if (lastSize > 0 && angles.size() > lastSize)
                    offset = angles.size() - lastSize;
                return std::accumulate(angles.begin() + offset, angles.end(), 0.f) / (float) (angles.size() - offset);
            }

            float getMeanAngleDiffs(int lastSize = 0)
            {
                int offset = 0;
                if (lastSize > 0 && angles.size() > lastSize)
                    offset = angles.size() - lastSize;
                return std::accumulate(angleDiffs.begin() + offset, angleDiffs.end(), 0.f) / ((float)angleDiffs.size() - offset);
            }

            float getEstimatedAngle(int n = 20)
            {
                return angles.back() + getMeanAngleDiffs(n);
            }
        };

        Road build2(const cv::Mat& cannied, size_t maxsize)
        {
            Road road(center.x, getFirstFragment2(cannied));
            optional<POINT> newPoint;
            while ((newPoint = getNextPoint(cannied, road.getEstimatedAngle(), road.points.back(),2)).valid && road.points.size() < maxsize)
            {
                if (!road.addPoint(*newPoint))
                    break;
            }

            return road;
        }
    };


}