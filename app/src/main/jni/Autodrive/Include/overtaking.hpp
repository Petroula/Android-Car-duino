#include "sensordata.hpp"

namespace Autodrive {
    namespace Overtaking {
        int usFront = SensorData::ultrasound.front;
        int objectDistance = 50;

        command run(command lastCommand) {
            if (usFront > 0 && usFront < objectDistance) {
                if (overtaking < 1) overtaking = lastCommand.setSpeed(0);
            }

             return lastCommand;
        }
    }
}