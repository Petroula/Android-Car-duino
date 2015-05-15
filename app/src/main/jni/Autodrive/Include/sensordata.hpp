#pragma once
#include <opencv2/core/mat.hpp>

namespace Autodrive
{
    namespace SensorData
    {
        struct ultrasound_t {
            int front;
            int frontright;
            int rear;
        } ultrasound = { 0, 0, 0 };

        struct infrared_t {
            int frontright;
            int rearright;
            int rear;
        } infrared = { 0, 0, 0 };

        double PULSES_PER_CM = 1;     //to be determined experimentally 1 for sim
        //int ultrasound[] = { 0, 0, 0 }; // front, frontright, rear
        //int infrared[] = { 0, 0, 0 };   // frontright, rearright, rear
        long encoderPulses = 0;
        
		// required for parking
        //int irFrontRight = infrared[0];
        //int irRearRight = infrared[1];
        //int irRear = infrared[2];

        //int usFront = ultrasound[0];
        //int usFrontRight = ultrasound[1];
        //int usRearRight = ultrasound[2];
        
        int razorHeading = 0;   // from -180 to 180
        int gyroHeading = 0;    // from 0 to 360
        
		int currentSpeed = 0;
        int currentAngle = 0; 
		// --------------------
		
        double encoderDistance()
        {
            return encoderPulses / PULSES_PER_CM;
        }
        cv::Mat* image = 0;
  
    };

}
