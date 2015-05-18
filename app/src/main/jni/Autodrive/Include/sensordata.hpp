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

        double PULSES_PER_CM = 1;
        long encoderPulses = 0;
        
        int razorHeading = 0;   // from -180 to 180
        int gyroHeading = 0;    // from 0 to 360
        
		int currentSpeed = 0;
        int currentAngle = 0; 
        
        int carLength = 1;
    
        void setCarLength(int length)
        {
            carLength = length;
        }
		
        double encoderDistance()
        {
            return encoderPulses / PULSES_PER_CM;
        }
        cv::Mat* image = 0;
  
    };

}
