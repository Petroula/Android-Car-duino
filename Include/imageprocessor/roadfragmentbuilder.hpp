/*

THIS FILE IS DEPRECATED AND ONLY CHECKED IN FOR HISTORICAL PURPOSES

*/
#pragma once
#include "util.hpp"
#include "line.hpp"
#include "birdseyetransformer.hpp"

namespace Autodrive
{
    typedef cv::Point_ < float > POINT;

    static const int distanceToRoad()
    {
        return 20;
    }

    static POINT recalibrate_center(POINT right, float dir)
    {
        return right + POINT(::std::cos(dir + Mathf::PI_2) * distanceToRoad(), -::std::sin(dir + Mathf::PI_2) * distanceToRoad());
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
                search_result = firstnonzero_direction(cannied, center, angle - Mathf::PI_2, 360);
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
        const float PI = Mathf::PI;
        const float PI_2 = Mathf::PI_2;
        typedef ::std::vector<roadfragment> roadfragments;

    public:



        static roadfragment getFirstFragment(const cv::Mat& cannied, POINT start_point)
        {
            SearchResult right;
            int unfound = 0;
            float targetdir = Mathf::PI_2;//UP

            //SEARCH UPWARDS UNTIL HIT
            while (!right.found && unfound++ < 9)
            {
                right = firstnonzero_direction(cannied, start_point, targetdir - Mathf::PI_2, 360);// 0 = RIGHT
                if (!right.found)
                    start_point.y--;
            }

            POINT recalibrated_center = recalibrate_center(right.point, targetdir);
            right.distance = distanceToRoad();

            // SEARCH DOWNWARDS IF HIT ON FIRST Y AXIS
            SearchResult new_right;
            new_right = right;
            int yStart = 0;
            while (new_right.found && unfound == 1 && yStart++ < 5 && recalibrated_center.y < cannied.size().width - 10)
            {
                new_right = firstnonzero_direction(cannied, recalibrated_center, targetdir - Mathf::PI_2, 360);// 0 = RIGHT
                if (new_right.found && new_right.distance == distanceToRoad())
                {
                    recalibrated_center.y++;
                    right = new_right;
                } else
                    break;
            }

            return roadfragment(recalibrated_center, right.point, Mathf::PI_2, right.distance); // M_PI_2 = FORWARD
        }



        static optional<roadfragment> findFragment(const cv::Mat& cannied, POINT start_point, const roadfragment& prevSegment)
        {
            SearchResult right = firstnonzero_direction(cannied, start_point, prevSegment.angle - Mathf::PI_2, 360);
            //new_direction = last_dir - (right.distance - prevSegment.distance) / 1.5f;
            float new_direction = linef(prevSegment.right, right.point).direction();
            roadfragment newfragment(start_point, right.point, new_direction, right.distance);
            return newfragment;
        }

        static optional<roadfragment> getNextFragment(const cv::Mat& cannied, const roadfragment& prevSegment)
        {
            int tries = 0;
            float last_dir = prevSegment.angle;
            POINT center = prevSegment.center;
            roadfragment nextFragment;
            while (true)
            {

                center += POINT(std::cos(last_dir), -std::sin(last_dir));
                optional<roadfragment> fragment = findFragment(cannied, center, prevSegment);

                // Safely keep looking
                /*if (right.found && abs(new_direction - last_dir) < 1.f)// && right.distance == prevSegment.distance)
                {
                last_dir = new_direction;
                continue;
                }*/

                if (fragment.valid && abs(fragment->distance - prevSegment.distance) < 2
                    //&& linef(right.point,prevSegment.right).length2() < 4
                    && abs(fragment->angle - last_dir) < 0.3f)
                {
                    nextFragment = *fragment;
                    break;
                } else
                {
                    tries++;
                    if (tries > 30)
                        return nullptr;
                }
            }

            nextFragment.angle = weighted_average<float>(last_dir, nextFragment.angle, 2.);
            nextFragment.center = recalibrate_center(nextFragment.right, nextFragment.angle);
            return nextFragment;
        }

        static SearchResult findFragment2(const cv::Mat& cannied, POINT start, float leftAngle, float rightAngle)
        {
            SearchResult rightSearch = firstnonzero_direction(cannied, start, rightAngle, 16);
            SearchResult leftSearch = firstnonzero_direction(cannied, start, leftAngle, 16);
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

        static optional<roadfragment> getNextFragment2(const cv::Mat& cannied, const roadfragment& prevSegment)
        {

            POINT it = prevSegment.right;
            bool vertical_search = false;
            if (prevSegment.angle > (Mathf::PI*3.f) / 4.f)
                vertical_search = true;
            float left_dir = Mathf::PI;
            float right_dir = 0;
            if (vertical_search)
            {
                it.x--;
                left_dir += Mathf::PI_2;
                right_dir += Mathf::PI_2;
            } else
            {
                it.y--;
            }
            auto searchResult = findFragment2(cannied, it, left_dir, right_dir);
            float angle = linef(prevSegment.right, searchResult.point).direction();
            return roadfragment(searchResult.point, searchResult.point, angle, searchResult.distance);
        }

        static roadfragment getFirstFragment2(const cv::Mat& cannied, POINT start_point)
        {
            SearchResult searchRes;
            int unfound = 0;

            float right = 0.f;
            float up = Mathf::PI_2;
            float left = Mathf::PI;
            float down = Mathf::PI + Mathf::PI_2;


            //SEARCH UPWARDS UNTIL HIT
            while (!searchRes.found && unfound++ < 9)
            {
                searchRes = findFragment2(cannied, start_point, left,right);
                if (!searchRes.found)
                    start_point.y--;
            }

            POINT recalibrated_center = recalibrate_center(searchRes.point, up);
            searchRes.distance = distanceToRoad();

            // SEARCH DOWNWARDS IF HIT ON FIRST Y AXIS
            SearchResult new_hit;
            new_hit = searchRes;
            int yStart = 0;
            while (new_hit.found && unfound == 1 && yStart++ < 5 && recalibrated_center.y < cannied.size().width - 10)
            {
                new_hit = findFragment2(cannied, start_point, left,right);
                if (new_hit.found && new_hit.distance == distanceToRoad())
                {
                    recalibrated_center.y++;
                    searchRes = new_hit;
                } else
                    break;
            }

            return roadfragment(recalibrated_center, searchRes.point, Mathf::PI_2, searchRes.distance); // M_PI_2 = FORWARD
        }

        /*static void markings(cv::Mat cannied)
        {
        ::std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(cannied, lines, 1, CV_PI / 180, 20, 50, 50);
        int leftmost = cannied.size().width;
        int rightmost = 0;
        bool foundLeft = false;
        bool foundRight = false;
        int center = cannied.size().width / 2;

        //#ifdef DEBUG_LANE_MARKINGS
        cv::Mat colorCopy;
        cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);
        //#endif

        static linef lastLML;
        static linef lastRML;
        for (cv::Vec4i line : lines)
        {
        int leftx = line[0];
        int rightx = line[2];
        int boty = line[1];
        int topy = line[3];
        linef vector(line);
        int thresh = 1;
        bool yes = false;

        float dirr = vector.direction_fixed_half();

        float dir_diff = dirr - Mathf::PI_2;

        if (abs(dir_diff) > 0.6f)
        continue;

        vector.draw(colorCopy);
        }


        cv::resize(colorCopy, colorCopy, colorCopy.size() * 2);//resize image
        cv::namedWindow("Lane_Markings", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
        cv::imshow("Lane_Markings", colorCopy);
        }*/


        static roadfragments build(const cv::Mat& cannied, size_t maxsize, POINT start_point)
        {
            roadfragments fragments;

            roadfragment prevSegment = getFirstFragment2(cannied, start_point);
            fragments.push_back(prevSegment);
            optional<roadfragment> segment;

            while ((segment = getNextFragment2(cannied, prevSegment)).valid && fragments.size() < maxsize)
            {
                fragments.push_back(*segment);
                prevSegment = *segment;
            }


            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            std::cout << fragments.size() << std::endl;

            for (roadfragment& frag : fragments)
            {
                linef(frag.center, frag.right).draw(colorCopy);
            }

            auto lastfrag = fragments[fragments.size() - 1];

            linef(lastfrag.right, lastfrag.right + POINT(cos(lastfrag.angle)*20.f, -sin(lastfrag.angle)*20.f)).draw(colorCopy);


            cv::resize(colorCopy, colorCopy, colorCopy.size() * 3);//resize image
            cv::namedWindow("mainwindow2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
            cv::imshow("mainwindow2", colorCopy);

            return fragments;
        }

    };
}
