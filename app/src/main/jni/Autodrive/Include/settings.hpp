#pragma once
namespace Autodrive
{
    namespace Settings
    {
        bool normalizeLightning = true;
        //Maximum vertical distance to the first pixel from carY
        int firstFragmentMaxDist = 32;//15-60
        //How many pixels to iterate to the left, for each pixel
        int leftIterationLength = 10;//1-15
        //How many pixels to iterate to the right, for each pixel
        int rightIterationLength = 6;//1-15
        //How many pixels of the transform border to remove from the canny
        int transformLineRemovalThreshold = 17;
        // If the middle line should be taken into consideration or not
        bool useLeftLine = true;
        // How much less to iterate right and left when finding the first point
        float iterateReduceOnStart = -1.f;
        // Every pixel in a line can not have an angle from the previous pixel that deviates more than this
        float maxAngleDiff = 0.65f; // 0.4 - 1.4
        // N Frames to take the mean value from
        uint smoothening = 2; // 0 - 8v

        //Number of frames the road needs to be unfound for resetting the angle, 0 means never
        int goForwardWhenLost = 3;
        // PID SETTINGS
        float kp = 0.5;
        float ki = 0.0;
        float kd = 0.0;
    }
}

