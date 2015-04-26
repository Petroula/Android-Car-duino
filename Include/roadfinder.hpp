#include "line.hpp"
#include <queue>
#include "roadfragmentbuilder.hpp"
#include <numeric>
#include "lightnormalizer.hpp"
#include "roadpiece.hpp"

namespace Autodrive
{
    class roadfinder
    {
        typedef cv::Point_ < float > POINT;
        const float PI = Math<float>::PI;
        const float PI_2 = Math<float>::PI_2;
        typedef ::std::vector<roadfragment> FRAGMENTS;
        typedef ::std::vector<roadpiece> roadparts_t;

        roadparts_t roadParts;

        int canny_thresh1;
        int canny_thresh2;

    public:

        int updateFrequency;
        int updateCounter;

        float getWay(float f)
        {
            if (f < 0.f)
                return -1;
            if (f == 0.f)
                return 0;
            if (f > 0.f)
                return 1;
            throw;
        }

        POINT start_center;

        piece_type previes_type = STRAIGHT;

        roadparts_t findRoadParts(const cv::Mat& cannied)
        {
            roadparts_t new_roadparts;
            FRAGMENTS road = roadfragmentbuilder::build(cannied, 100, start_center);
            start_center = road[0].center;
            previes_type = NONE;
            //DENOISE 3
            int hits = 0;
            int last_way = -1;
            std::vector<float> lastDiffs;
            for (unsigned int i = 0; i < road.size() - 1; ++i)
            {
                POINT diff = road[i + 1].right - road[i].right;
                lastDiffs.push_back(getWay(diff.x) / (-diff.y));
                const int queSize = 10;
                int prev_end = 0;
                if (lastDiffs.size() > queSize)
                {
                    if (i < queSize)
                        continue;

                    float sum = std::accumulate(lastDiffs.begin(), lastDiffs.end(), 0.f);
                    piece_type new_type;

                    if (sum < -2.f)
                        new_type = LEFT_TURN;
                    else if (sum > 2.f)
                        new_type = RIGHT_TURN;
                    else
                        new_type = STRAIGHT;

                    if (previes_type == new_type)
                        continue;

                    if ((previes_type == LEFT_TURN || previes_type == RIGHT_TURN) &&
                        (new_roadparts.back().end.right.y - road[i].right.y) < 5.f)
                        continue;

                    int begin = i - queSize;
                    if (!new_roadparts.empty())
                        new_roadparts.back().end = road[begin];
                    else
                        begin = 0;

                    new_roadparts.push_back(roadpiece(road[begin], road[i], new_type));
                    prev_end = i;
                    previes_type = new_type;
                    lastDiffs.erase(lastDiffs.begin());
                }
            }

            if (!new_roadparts.empty())
                new_roadparts.back().end = road.back();
            else
                new_roadparts.push_back(roadpiece(road.front(), road.back(), STRAIGHT));

            return new_roadparts;
        }

        void build(const cv::Mat& cannied, POINT start_point, int _updateFrequency)
        {
            updateFrequency = _updateFrequency;
            start_center = start_point;
            roadParts = findRoadParts(cannied);

            /*
            DENOISE2
            for (auto it = road.begin() +1; it != road.end(); ++it)
            {
            auto lastSame = it;
            for (auto it2 = it; it2 != road.end(); ++it2)
            {
            linef line(it->right, it2->right);
            float diff = line.length2();
            //if (diff > 100.f)
            //   break;
            float div = 1.f + diff;
            if (abs(line.direction() - it->angle) / div   < 0.16f
            && abs(it->angle - it2->angle) / div < 0.16f)
            {
            lastSame = it2;
            }
            }

            if (lastSame != it)
            {
            it = road.erase(it, lastSame - 1);
            }
            }
            */


            //DENOISE 1
            /*if (road.size() >= 3)
            {
            for (unsigned int i = 0; i < road.size() - 3; i++){
            // REMOVE ALL COMBINATIONS OF increases directly after decreases and viceversa
            float ang0 = road[i].angle;// Comparison angle
            float ang1 = road[i+1].angle;
            float ang2 = road[i+2].angle;
            auto line = linef(road[i].right, road[i + 2].right);

            if ((ang1 > ang0 && ang2 < ang0 ||
            ang1 < ang0 && ang2 > ang0)
            && line.length2() <= 6.f){
            road.erase(road.begin() + i + 1);
            road.erase(road.begin() + i + 2);
            i++;
            }
            }
            }*/
        }

        float getAngle()
        {
            float angle = roadParts[0].end.angle;
            return angle;
        }

        void shiftDown(float speed)
        {
            for (roadpiece& piece : roadParts)
            {
                piece.shiftDown(start_center, speed);
            }

            if (!roadParts.empty() && roadParts[0].finished)
            {
                roadParts.erase(roadParts.begin());
            }
        }

        void stabilize(cv::Mat& cannied, float est_speed = 1.f)
        {
            //POINT diff = roadParts[0].stabilize(cannied);
            //diff.y += 5.f;

            for (roadpiece& piece : roadParts)
            {
                piece.stabilize(cannied);
                //piece.begin.center += diff;
                //piece.begin.right += diff;                
            }

            /*


           int indexHalf = road.size() / 2;

           roadfragment newSegment = *getNextFragment(cannied, road[indexHalf - 1]);
           roadfragment currSegment = road[indexHalf];

           POINT diff_right = newSegment.right - currSegment.right;



           updateSegment(cannied, indexHalf);
           updateSegment(cannied, indexHalf - indexHalf / 2);
           updateSegment(cannied, indexHalf + indexHalf / 2);
           */
            //POINT diff_center = compareSegment.center - current.center;

        }

        void draw(const cv::Mat& cannied)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            for (roadpiece& piece : roadParts)
            {
                piece.draw(colorCopy);
            }

            linef(start_center, POINT(start_center.x, 0)).draw(colorCopy);

            cv::resize(colorCopy, colorCopy, colorCopy.size() * 3);//resize image
            cv::namedWindow("mainwindow", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
            cv::imshow("mainwindow", colorCopy);
        }

        void update(cv::Mat& cannied, bool draw_road, float estimated_speed)
        {
            /*if (updateCounter++ >= updateFrequency)
            {
            updateCounter = 0;
            */
            roadParts = findRoadParts(cannied);
            //}

            /*
                optional<roadfragment> nextSegment;
                if (nextSegment = getNextFragment(cannied, road.back()))
                {
                road.push_back(nextSegment);
                }

                if (road.size()>8){
                shiftDown();
                road.erase(road.begin());
                }
                */
            //shiftDown(estimated_speed);

            //stabilize(cannied, 1.f);

            if (draw_road)
            {
                draw(cannied);
            }
        }

    };

}