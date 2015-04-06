#pragma once
#include <string.h>

namespace Autodrive {

    struct command{
        bool changedAngle = false;
        bool changedSpeed = false;
        double angle = 0.0;
        double speed = 0.0;
        inline void setSpeed(double newspeed) {
            changedSpeed = true;
            speed = newspeed;
        }
        inline void setAngle(double newangle){
            changedAngle = true;
            angle = newangle;
        }
    };

}
