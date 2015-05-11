#pragma once
#include <opencv2/core/mat.hpp>

namespace Autodrive
{
    namespace SensorData
    {
        enum UltrasoundSensor
        {
            FRONT = 0,
            FRONT_RIGHT = 1,
            REAR = 2
        };

        double PULSES_PER_CM = 1; //to be determined experimentally
        int ultrasound[] = { 0, 0, 0 };
        int infrared[] = { 0, 0, 0 };
        long encoderPulses = 0;
        double encoderDistance()
        {
            return encoderPulses;
        }
        cv::Mat* image = 0;
    };

}