#pragma once
#include "util.hpp"

namespace Autodrive
{
    typedef cv::Point_ < float > POINT;

    static const int distanceToRoad()
    {
        return 20;
    }

    static POINT recalibrate_center(POINT right, float dir)
    {
        return right + POINT(::std::cos(dir + Math<float>::PI_2) * distanceToRoad(), -::std::sin(dir + Math<float>::PI_2) * distanceToRoad());
    }

    struct roadfragment
    {
        POINT center;
        POINT right;
        float angle;
        int distance;
        roadfragment() {}
        roadfragment(POINT center_point, POINT right_point, float _angle, int dist) :
            center(center_point), right(right_point), angle(_angle), distance(dist)
        {}

        void Update(const cv::Mat& cannied)
        {
            SearchResult search_result;
            int unfound = 0;
            while (unfound++ < 9)
            {
                search_result = firstnonzero_direction(cannied, center, angle - Math<float>::PI_2);
                if (search_result.found)
                {
                    right = search_result.point;
                    center = recalibrate_center(right, angle);
                    distance = search_result.distance;
                    break;
                } else
                    center.y--;
            }

        }
    };

    class roadfragmentbuilder
    {
        typedef cv::Point_ < float > POINT;
        const float PI = Math<float>::PI;
        const float PI_2 = Math<float>::PI_2;
        typedef std::vector<roadfragment> roadfragments;

    public:

        static roadfragment getFirstFragment(const cv::Mat& cannied, POINT start_point)
        {
            SearchResult right;
            int unfound = 0;
            float targetdir = Math<float>::PI_2;//UP

            //SEARCH UPWARDS UNTIL HIT
            while (!right.found && unfound++ < 9)
            {
                right = firstnonzero_direction(cannied, start_point, targetdir - Math<float>::PI_2);// 0 = RIGHT
                if (!right.found)
                    start_point.y--;
            }

            POINT recalibrated_center = recalibrate_center(right.point, targetdir);
            right.distance = distanceToRoad();

            // SEARCH DOWNWARDS IF HIT ON FIRST Y AXIS
            SearchResult new_right;
            new_right = right;
            while (new_right.found && unfound == 1)
            {
                new_right = firstnonzero_direction(cannied, recalibrated_center, targetdir - Math<float>::PI_2);// 0 = RIGHT
                if (new_right.found && (new_right.distance - distanceToRoad() < 1))
                {
                    recalibrated_center.y++;
                    right = new_right;
                } else
                    break;
            }


            right.distance = 20;

            return roadfragment(recalibrated_center, right.point, Math<float>::PI_2, right.distance); // M_PI_2 = FORWARD
        }

        static optional<roadfragment> getNextFragment(const cv::Mat& cannied, const roadfragment& prevSegment)
        {
            int tries = 0;
            float last_dir = prevSegment.angle;
            POINT center = prevSegment.center;
            float new_direction;
            SearchResult right;
            while (true)
            {
                center += POINT(std::cos(last_dir), -std::sin(last_dir));
                right = firstnonzero_direction(cannied, center, last_dir - Math<float>::PI_2);
                new_direction = linef(prevSegment.right, right.point).direction();

                // Safely keep looking
                if (right.found && abs(new_direction - last_dir) < 0.01f)
                    continue;

                if (right.found && abs(right.distance - prevSegment.distance) < 8 && abs(new_direction - last_dir) < 0.3f)
                {
                    break;
                } else
                {
                    tries++;
                    if (tries > 3)
                        return nullptr;
                }
            }

            new_direction = weighted_average<float>(last_dir, new_direction, 2.);
            POINT recalibrated_center = recalibrate_center(right.point, new_direction);
            return roadfragment(recalibrated_center, right.point, new_direction, right.distance);
        }


        static roadfragments build(const cv::Mat& cannied, size_t maxsize, POINT start_point)
        {
            roadfragments fragments;

            roadfragment prevSegment = getFirstFragment(cannied, start_point);
            fragments.push_back(prevSegment);
            optional<roadfragment> segment;
            while ((segment = getNextFragment(cannied, prevSegment)).valid && fragments.size() < maxsize)
            {
                fragments.push_back(*segment);
                prevSegment = *segment;
            }


            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);


            for (roadfragment& frag : fragments)
            {
                linef(frag.center, frag.right).draw(colorCopy);
            }

            auto lastfrag = fragments[fragments.size()-1];

            linef(lastfrag.right, lastfrag.right + POINT(cos(lastfrag.angle)*20.f, -sin(lastfrag.angle)*20.f)).draw(colorCopy);
            

            cv::resize(colorCopy, colorCopy, colorCopy.size() * 3);//resize image
            cv::namedWindow("mainwindow2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
            cv::imshow("mainwindow2", colorCopy);
            

            return fragments;
        }

    };
}