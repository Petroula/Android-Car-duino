#include "roadfragmentbuilder.hpp"
namespace Autodrive
{
    enum piece_type
    {
        NONE,LEFT_TURN, RIGHT_TURN, STRAIGHT
    };


    struct roadpiece
    {
        bool reached_bottom = false;
        bool finished = false;
        roadfragment begin;
        roadfragment end;
        piece_type kind;
        roadpiece(roadfragment begin_roadfragment, roadfragment end_roadfragment, piece_type type) :
            begin(begin_roadfragment), end(end_roadfragment), kind(type)
        {}

        void draw(cv::Mat& colorCopy)
        {
            switch (kind)
            {
                case Autodrive::LEFT_TURN:
                {
                    POINT pBegin = begin.center;
                    POINT pEnd = end.center;
                    /*pEnd.y /= 2.f;
                    pEnd.x -= 10.f;

                    float angleDiff = begin.angle - end.angle;
                    POINT diff = pBegin - pEnd;

                    /// REVERSE HEIGHT + WIDTH
                    float H = diff.x;
                    float W = -diff.y;
                    float radius = (H / 2) + ((W*W) / (H * 8));
                    float arcLength = angleDiff * radius*3.14f;
                    float angleDeltaPerStep = angleDiff / arcLength;
                    
                    float angle = begin.angle;//UP
                    
                    for (auto p = pBegin; angle < end.angle;)
                    {
                        p.y -= sin(angle);
                        p.x += cos(angle);
                        angle += angleDeltaPerStep;
                        linef(p, p).draw(colorCopy);
                    }

                    cv::circle(colorCopy, POINT(pEnd.x, pEnd.y), 1, cv::Scalar(0, 0, 255));*/

                    linef(pBegin, pEnd).draw(colorCopy,cv::Scalar(0, 0, 255));
                    linef(pEnd, pEnd + POINT(cos(end.angle) * 20, -sin(end.angle) * 20)).draw(colorCopy, cv::Scalar(255, 255, 0));
                    break; 
                }
                case Autodrive::RIGHT_TURN:
                    break;
                case Autodrive::STRAIGHT:
                    cv::rectangle(colorCopy, begin.center, end.right, cv::Scalar(0, 255, 0));
                    break;
                default:
                    break;
            }
            //linef(road[i].center, road[i].right).draw(colorCopy, cv::Scalar(0, 0, 255), 1);
            //linef(begin.right, end.right).draw(colorCopy, cv::Scalar(0, 0, 255), 1);
        }

        POINT stabilize(cv::Mat& cannied_mat)
        {
            roadfragment prev_end = end;
            begin.Update(cannied_mat);
            end.Update(cannied_mat);
            return end.right - prev_end.right;
        }

        void shiftDown(POINT start_point, float speed)
        {
            end.center.y += speed;
            if (!reached_bottom)
            {
                begin.center.y += speed;
                if (begin.center.y >= start_point.y)
                {
                    reached_bottom = true;
                }
            } else
            {
                if (end.center.y >= begin.center.y)
                {
                    finished = true;
                }
            }

        }
    };
}