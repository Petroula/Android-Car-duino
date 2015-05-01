#pragma once
#include <string.h>
#include "imageprocessor.hpp"

namespace Autodrive {

    struct command{
        bool changedAngle = false;
        bool changedSpeed = false;
        int angle = 0;
        int speed = 0;
        inline void setSpeed(int newSpeed)
        {
            changedSpeed = true;
            speed = newSpeed;
        }
        inline void setAngle(int newAngle)
        {
            assert(angle >= -25 && angle <= 25);
            changedAngle = true;
            angle = newAngle;
        }
    };

}
