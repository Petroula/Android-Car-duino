#pragma once
#include <string.h>
#include "imageprocessor.hpp"

namespace Autodrive {

    struct command{
        bool changedAngle = false;
        bool changedSpeed = false;
        int angle = 0;
        int speed = 10;
        inline void setSpeed(int speed)
        {
            changedSpeed = true;
            this->speed = speed;
        }
        inline void setAngle(int angle)
        {
            assert(angle >= -25 && angle <= 25);
            changedAngle = true;
            this->angle = angle;
        }
    };

}