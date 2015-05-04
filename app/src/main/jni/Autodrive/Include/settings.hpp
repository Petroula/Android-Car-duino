#pragma once
namespace Autodrive
{
    namespace Settings
    {
        bool normalizeLightning = true;
        //Maximum vertical distance to the first pixel from carY
        int firstFragmentMaxDist = 30;//15-60
        //How many pixels to iterate to the left, for each pixel
        int leftIterationLength = 6;//1-15
        //How many pixels to iterate to the right, for each pixel
        int rightIterationLength = 8;//1-15
        bool useLeftLine = true;
        // Every pixel in a line can not have an angle from the previous pixel that deviates more than this
        float maxAngleDiff = 0.8f; // 0.4 - 1.4
        // N Frames to take the mean value from
        uint smoothening = 3; // 0 - 8v
    }
}