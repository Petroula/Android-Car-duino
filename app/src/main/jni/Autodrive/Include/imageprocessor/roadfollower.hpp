#include "linefollower.hpp"

namespace Autodrive
{
    class roadfollower
    {
        int carY = 0;
        int centerX = 0;
        std::unique_ptr<linefollower> leftLineFollower = nullptr;
        std::unique_ptr<linefollower> rightLineFollower = nullptr;
        
        std::vector<int> prevDirs;

        int FindCarEnd(const cv::Mat& cannied)
        {
            POINT center_bottom(centerX, cannied.size().height - 5);
            //SEARCH UPWARDS UNTIL _NOT_ HIT ON THE CENTER +/- 10
            bool hit = true;
            while (hit)
            {
                hit = firstnonzero_direction(cannied, center_bottom, Direction::RIGHT, 10).found
                    || firstnonzero_direction(cannied, center_bottom, Direction::LEFT, 10).found;
                if (hit)
                    center_bottom.y--;
            }
            center_bottom.y--;
            return center_bottom.y;
        }


        POINT FindLineStart(const cv::Mat& cannied, float direction)
        {
            POINT iter(centerX, carY);
            SearchResult searchRes;
            //SEARCH UPWARDS UNTIL HIT ON THE RIGHT or LEFT (dependant on direction value)
            while (!searchRes.found)
            {
                searchRes = firstnonzero_direction(cannied, iter, direction, 360);// 0 = RIGHT
                if (!searchRes.found)
                    iter.y--;
            }
            return searchRes.point;
        }

        cv::Mat draw(const cv::Mat& cannied)
        {
            cv::Mat colorCopy;
            cv::cvtColor(cannied, colorCopy, CV_GRAY2RGB);

            leftLineFollower->draw(&colorCopy,centerX);
            rightLineFollower->draw(&colorCopy, centerX);

            return colorCopy;
        }


    public:
        roadfollower(const cv::Mat& cannied, int center_x) : centerX(center_x)
        {
            carY = FindCarEnd(cannied);
            POINT rightLineStart = FindLineStart(cannied, Direction::RIGHT);
            POINT leftLineStart = FindLineStart(cannied, Direction::LEFT);

            leftLineFollower = make_unique<linefollower>(cannied, leftLineStart, centerX);
            rightLineFollower = make_unique<linefollower>(cannied, rightLineStart, centerX);
        }


        command update(cv::Mat& cannied, cv::Mat& drawMat)
        {
            command cmd;

            leftLineFollower->update(cannied);
            rightLineFollower->update(cannied);

            drawMat = draw(cannied);

            optional<int> leftTargetAngle = leftLineFollower->getPreferedAngle();
            optional<int> rightTargetAngle = rightLineFollower->getPreferedAngle();
            optional<int> targetAngle = nullptr;

            if (leftTargetAngle && rightTargetAngle && Settings::useLeftLine)
            {
                // Give the right line just a bit more priority since it seems more reliable
                targetAngle = weighted_average(*rightTargetAngle, *leftTargetAngle, 3);
            } else if (leftTargetAngle && Settings::useLeftLine)
            {
                targetAngle = *leftTargetAngle;
            } else if (rightTargetAngle)
            {
                targetAngle = *rightTargetAngle;
            }
            
            if(targetAngle)
            {

                if(Settings::smoothening == 0)
                {
                    cmd.setAngle(*targetAngle);
                }
                else
                {
                    int sum = (std::accumulate(prevDirs.begin(), prevDirs.end(), 0) + *targetAngle);
                    int newAngle = sum / float(prevDirs.size() + 1);
                    prevDirs.push_back(newAngle);
                    if(prevDirs.size() > Settings::smoothening)
                        prevDirs.erase(prevDirs.begin());
                    
                    cmd.setAngle(newAngle);
                }
            }
            

            return cmd;
        }


    };

}
