#pragma once
#include <opencv2/core/core.hpp>
#include <memory>

namespace Autodrive
{
    using POINT = cv::Point2f;

    struct Mathf
    {
        static const float PI;
        static const float PI_2;
        static int toDegrees(float radians)
        {
            return int(radians * 180.f / PI);
        }
    };

    const float Mathf::PI = acosf(-1);
    const float Mathf::PI_2 = PI / 2.f;

    struct Direction
    {
        static const float RIGHT;
        static const float LEFT;
        static const float FORWARD;
    };

    const float Direction::RIGHT = 0.f;
    const float Direction::LEFT = Mathf::PI;
    const float Direction::FORWARD = Mathf::PI_2;

    
    #ifndef __ANDROID__
        void show_image(cv::Mat mat, int resize, std::string wName)
        {
            cv::resize(mat, mat, mat.size() * resize);//resize image
            cv::imshow(wName, mat);
        }
    #endif
    
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) 
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<class TYPE>
    struct optional
    {
        optional(TYPE value) : valid(true), val(value)
        {

        }
        optional() : valid(false)
        {

        }
        optional(::std::nullptr_t) : valid(false)
        {

        }
        explicit operator bool() const
        {
            return valid;
        }
        TYPE* operator->()
        {
            return &val;
        }
        TYPE operator* () const
        {
            return val;
        }
        bool valid;
    private:
        TYPE val;
    };

    struct SearchResult
    {
        POINT point;
        int distance;
        bool found = false;
    };

    SearchResult firstnonzero_direction(const cv::Mat& mat, cv::Point_ < float > start, float direction, int maxDist)
    {
        SearchResult res;
        POINT new_pos = start + POINT(::std::cos(direction) * maxDist, -::std::sin(direction) * maxDist);
        cv::LineIterator it(mat, start, new_pos);
        for (int i = 0; i < it.count; i++, it++)
        {
            if (mat.at<uchar>(it.pos()))
            {
                res.found = true;
                res.distance = i;
                res.point = it.pos();
                break;
            }
        }
        return res;
    }

    optional<cv::Point> firstnonzero_horizontal(const cv::Mat& mat, cv::Point iterator)
    {
        while (iterator.x < mat.size().width - 1)
        {
            if (mat.at<uchar>(iterator) != 0)
            {
                return iterator;
            }
            iterator.x++;
        }
        return nullptr;
    }

    template<class numeric_t>
    numeric_t weighted_average(numeric_t val1, numeric_t val2, numeric_t val1_multiplier)
    {
        return (val1*val1_multiplier + val2) / (val1_multiplier + 1);
    }
}
