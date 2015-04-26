#pragma once
#include <opencv2/core/core.hpp>
#include <limits>
namespace Autodrive
{

    template <typename numeric_t>
    struct line
    {
        using pointT = cv::Point_ < numeric_t >;
        using vecT = cv::Vec < numeric_t, 4 >;
        using limitsT = std::numeric_limits < numeric_t >;

        pointT begin;
        pointT end;

        line(vecT line) : line(pointT(line[0], line[1]), pointT(line[2], line[3]))
        {}

        line(pointT begin, pointT end) : begin(begin), end(end)
        {
            compute();
        }

        line()
        {

        }

        void compute()
        {
            if (begin.x == end.x)
            {
                k = limitsT::has_infinity ? limitsT::infinity() : limitsT::max();
            } else
            {
                k = (begin.y - end.y) / (begin.x - end.x);
            }

            m = begin.y - begin.x * k;
        }

        float length()
        {
            return std::sqrt(length2());
        }

        float length2()
        {
            auto d = begin - end;
            return d.x *d.x + d.y *d.y;
        }

        void draw(cv::Mat& mat, cv::Scalar color = cv::Scalar(0, 255, 0), int thickness = 1)
        {
            cv::line(mat, begin, end, color, thickness);
        }

        float direction()
        {
            cv::Point2f diff = end - begin;
            return atan2(-diff.y, diff.x);
        }

        /*
        
            RETURNS A DIRECTION THAT IS ALLWAYS BETWEEN 0 - PI , (0-180 in degrees)

        */
        float direction_fixed_half()
        {
            float dirr = direction();
            if (dirr < 0.f)
            {
                dirr = Math<float>::PI + dirr;
            }

            if (dirr > Math<float>::PI)
            {
                dirr = dirr - Math<float>::PI;
            }

            return dirr;
        }

        bool differs_less_than_from(line<numeric_t> line2, numeric_t m_diff, numeric_t k_diff)
        {
            return abs(line2.k - k) < k_diff && abs(line2.m - m < m_diff);
        }

        void stretchY(numeric_t bottom, numeric_t top)
        {
            numeric_t ychangeBegin = begin.y - bottom;
            begin = pointT(begin.x - ychangeBegin / k, bottom);

            numeric_t ychangeTop = end.y - top;
            end = pointT(end.x - ychangeTop / k, top);
        }

        float k = 1;
        float m = 0;
    };

    typedef line <int> linei;
    typedef line <float> linef;
}