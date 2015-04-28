#pragma once
#include "line.hpp"
#include "util.hpp"
#include <numeric>

namespace Autodrive
{
    typedef cv::Point_ < float > POINT;

    class roadfragmentbuilder2
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


        static optional<POINT> getNextPoint(const cv::Mat& cannied, float est_angle, const POINT& prevPoint)
        {

            POINT it = prevPoint;
            bool vertical_search = false;
            //if (est_angle > (Mathf::PI*3.f) / 4.f)
            //    vertical_search = true;
            float left_dir = Mathf::PI;
            float right_dir = 0;
            SearchResult searchResult;
            int unfound = 0;
            while (!searchResult.found && unfound < 10)
            {
                if (vertical_search)
                {
                    it.x--;
                    left_dir += Mathf::PI_2;
                    right_dir += Mathf::PI_2;
                } else
                {
                    it.y--;
                    //if (unfound > 0)
                    it.x += cosf(est_angle);//*(0.4f + unfound / 10.f);
                }
                searchResult = findFragment2(cannied, it, left_dir, right_dir);
                
                unfound++;
            }

            if (searchResult.found)
                return searchResult.point;
            else
                return nullptr;
        }

    public:
        POINT last_start;
        POINT first_start;

        roadfragmentbuilder2(const cv::Mat& cannied, POINT start_point)
        {
            first_start = getFirstHit(cannied, start_point);
            last_start = first_start;
        }

        //TODO: refactor getter for mean angle etc
        struct Road
        {
            std::vector<POINT> points;
            std::vector<float> angles;
            std::vector<float> angleDiffs;

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
        };

        Road build2(const cv::Mat& cannied, size_t maxsize)
        {
            Road road;
            POINT prevPoint = getFirstFragment2(cannied);
            float prevAngle = Mathf::PI_2;
            float estimatedAngle = prevAngle;
            road.points.push_back(prevPoint);
            optional<POINT> newPoint;
            while ((newPoint = getNextPoint(cannied, estimatedAngle, prevPoint)).valid && road.points.size() < maxsize)
            {
                float newAngle = linef(prevPoint, *newPoint).direction_fixed_half();

                //Almost never happens
                float mean = road.getMeanAngle(20);
                if (abs(newAngle - mean) > Mathf::PI_2 / 1.4f)
                    break;

                road.angles.push_back(newAngle);
                road.angleDiffs.push_back(newAngle - prevAngle);
                estimatedAngle = newAngle + road.getMeanAngleDiffs(20);
                prevAngle = newAngle;
                road.points.push_back(*newPoint);
                prevPoint = *newPoint;
            }

            return road;
        }
    };


}