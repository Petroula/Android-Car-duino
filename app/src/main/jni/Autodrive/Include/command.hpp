#pragma once
#include <string.h>

namespace Autodrive {

    struct command{
        bool changedAngle = false;
        bool changedSpeed = false;
        double angle = 0;
        double speed = 0;
        inline void setSpeed(double newSpeed)
        {
            assert(newSpeed >= -1.0 && newSpeed <= 1.0);
            changedSpeed = true;
            speed = newSpeed;
        }
        inline void setAngle(double newAngle)
        {
            assert(newAngle >= -1.0 && newAngle <= 1.0);
            changedAngle = true;
            angle = newAngle;
        }
    };

}
