#pragma once
#include "line.hpp"
#include "util.hpp"
#include "../settings.hpp"
#include "roadline.hpp"

namespace Autodrive
{
    class roadlinebuilder
    {
        static const int pointDist = 2;
        static const int maxDistFromStart = 22;
        static const int maxUpwardsIteration = 15;//15
        int nTries = 0;
        int carY = 0;
        float angleIn = 0.f;
        float angleOut = 0.f;

        SearchResult FindPoint(const cv::Mat& cannied, POINT start,float iterationReduction = 0)
        {
            SearchResult outSearch = firstnonzero_direction(cannied, start, angleOut, Settings::rightIterationLength - iterationReduction
            + nTries);
            SearchResult inSearch = firstnonzero_direction(cannied, start, angleIn, Settings::leftIterationLength -iterationReduction + nTries*2);
            if (inSearch.found && outSearch.found)
            {
                if (inSearch.distance <= outSearch.distance + 20)
                {
                    {
                        return inSearch;
                    }
                } else
                {
                    return outSearch;
                }
            } else if (inSearch.found)
            {
                return inSearch;
            }

            return outSearch;
        }


        POINT GetFirstPoint(const cv::Mat& cannied)
        {
            SearchResult searchRes;
            int unfound = 0;

            POINT start_point = last_start;

            //SEARCH UPWARDS UNTIL HIT
            while (!searchRes.found && unfound++ < Settings::firstFragmentMaxDist)
            {
                searchRes = FindPoint(cannied, start_point,Settings::iterateReduceOnStart);
                if (!searchRes.found)
                    start_point.y-= 1 + nTries;
            }

            // SEARCH DOWNWARDS IF HIT ON FIRST Y AXIS
            SearchResult new_hit;
            new_hit = searchRes;
            int yStart = 0;
            while (new_hit.found && unfound == 1 && yStart++ < 5 && searchRes.point.y < carY)
            {
                new_hit.point.y++;
                new_hit = FindPoint(cannied, new_hit.point);
                if (new_hit.found && int(new_hit.point.x) == int(searchRes.point.x))
                {
                    searchRes = new_hit;
                } else
                    break;
            }

            if (linef(first_start, searchRes.point).length2() <= maxDistFromStart*maxDistFromStart)
                last_start = searchRes.point;

            return searchRes.point;
        }


        optional<POINT> GetNextPoint(const cv::Mat& cannied, float est_angle,float angle_derivate, const POINT& prevPoint,int delta = 2)
        {

            POINT it = prevPoint;
            SearchResult bestChoice;
            bestChoice.found = false;
            bestChoice.distance = 1000;
            int unfound = 0;

            while(unfound < maxUpwardsIteration * nTries)
            {
                it.y-=delta;
                it.x += cosf(est_angle)*delta;
                est_angle+=angle_derivate;
                SearchResult result = FindPoint(cannied, it);
                unfound++;

                if(result.found)
                {
                    if(result.distance == 0)
                    {
                        return result.point;
                    }else if(result.distance < bestChoice.distance)
                    {
                        bestChoice = result;
                    }
                }
            }
            if(bestChoice.found)
                return bestChoice.point;
            else
                return nullptr;
        }

        optional<POINT> GetNextPoint2(const cv::Mat& cannied, float est_angle,float angle_derivate, const POINT& prevPoint,int delta = 2)
        {
            POINT it = prevPoint;
            SearchResult searchResult;
            //untrigger unused warning
            angle_derivate++;
            int unfound = 0;
            while (!searchResult.found && unfound < maxUpwardsIteration)
            {
                it.y-=delta;
                it.x += cosf(est_angle)*delta;
                if(it.x > cannied.size().width/2 + 80)
                    it.x = cannied.size().width/2 + 80;
                searchResult = FindPoint(cannied, it);

                unfound++;
            }

            if (searchResult.found)
                return searchResult.point;
            else
                return nullptr;
        }

    public:
        POINT first_start;
        POINT last_start;
        float centerX;

        roadlinebuilder(POINT startPoint, float center_x,float angle_in,float angle_out) :
                angleIn(angle_in),angleOut(angle_out),first_start(startPoint), last_start(startPoint), centerX(center_x)
        {

        }

        optional<RoadLine> build(const cv::Mat& cannied, size_t maxsize)
        {
            optional<RoadLine> road = nullptr;
            for(nTries = 0; nTries < 3 && !road.valid; nTries++)
            {
                road = RoadLine(centerX, GetFirstPoint(cannied));
                optional<POINT> newPoint;
                while (road->points.size() < maxsize)
                {
                    newPoint = GetNextPoint(cannied, road->getEstimatedAngle(),road->getMeanAngleDiffs(), road->points.back(),pointDist);
                    if(!newPoint)
                        break;
                    if (!road->addPoint(*newPoint))
                        break;
                }
                if(!road->isFound())
                    road = nullptr;
            }

            return road;
        }
    };


}
