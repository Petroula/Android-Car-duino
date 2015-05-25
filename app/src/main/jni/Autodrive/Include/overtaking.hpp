#include "sensordata.hpp"

namespace Autodrive {
    namespace Overtaking {
        int usFront = SensorData::ultrasound.front;
        int objectDistance = 50;
        bool overtaking = false;

        command run(command lastCommand) {
            if (usFront > 0 && usFront < objectDistance) {
                if (! overtaking) overtaking = true;
            }

            if (overtaking) {
                lastCommand.setSpeed(0);
            }

             return lastCommand;
        }
    }
}