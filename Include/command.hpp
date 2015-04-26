#pragma once
#include <string.h>

namespace Autodrive {

    struct command{
        bool changedAngle = false;
        bool changedSpeed = false;
        double angle;
        double speed;
        inline void setSpeed(double speed){
            changedSpeed = true;
            this->speed = speed;
        }
        inline void setAngle(double angle){
            changedAngle = true;
            this->angle = angle;
        }
    };

}