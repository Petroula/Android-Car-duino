#pragma once
#include "sensordata.hpp"
#include "imageprocessor/imageprocessor.hpp"

namespace Autodrive
{
    command lastCommand;

    bool speedChanged() 
    {
        return lastCommand.changedSpeed;
    }

    bool angleChanged()
    {
        return lastCommand.changedAngle;
    }

    int getSpeed()
    {
        return lastCommand.speed;
    }

    float getAngle()
    {
        return lastCommand.angle;
    }

    enum carstatus
    {
        SEARCHING_FOR_LANES,FOLLOWING_LANES,UNKNOWN
    };

    carstatus status;

    void drive()
    {
        /* Reset command */
        lastCommand = command();

        switch (status)
        {
            case Autodrive::SEARCHING_FOR_LANES:
                if (Autodrive::imageProcessor::init_processing(*Autodrive::SensorData::image))
                    status = FOLLOWING_LANES;
                break;
            case Autodrive::FOLLOWING_LANES:
                lastCommand = Autodrive::imageProcessor::continue_processing(*Autodrive::SensorData::image);
                break;
            case Autodrive::UNKNOWN:
                /*
                
                EXAMPLE:

                if(Autodrive::SensorData::infrared[1] < 10)
                    lastCommand.setSpeed(0);

                */
                break;
            default:
                break;
        }
    }






}
