#pragma once
#include "sensordata.hpp"
#include "parking.hpp"
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

    double getSpeed()
    {
        return lastCommand.speed;
    }

    double getAngle()
    {
        return lastCommand.angle;
    }

    enum carstatus
    {
        DETECTING_GAP,PARKING,SEARCHING_FOR_LANES,FOLLOWING_LANES,UNKNOWN,DEBUG,DISTANCE_MEASURED_DEBUG
    };  
    
    carstatus status;

    void reset()
    {
        //status = SEARCHING_FOR_LANES;
        //status = DETECTING_GAP;
        status = DEBUG;
        //status = DISTANCE_MEASURED_DEBUG;
    }

    void drive()
    {
        /* Reset command */
        lastCommand = command();

        switch (status)
        {
            case Autodrive::SEARCHING_FOR_LANES:
                if (Autodrive::imageProcessor::init_processing(Autodrive::SensorData::image))
                {
                    lastCommand.setSpeed(0.28);
                    status = FOLLOWING_LANES;
                }
                break;
                
            case Autodrive::FOLLOWING_LANES:
                lastCommand = Autodrive::imageProcessor::continue_processing(*Autodrive::SensorData::image);
                break;
                
            // debug only! will be merged with lane following   
            case Autodrive::DETECTING_GAP:
                lastCommand = Parking::SetParkingProcedure();
                /*if(Parking::parkingProcedure == Parking::PERPENDICULAR_STANDARD){ // select parking procedure
                    status = PARKING;
                }else{
                    lastCommand.setSpeed(0.4); // this value breaks the simulation if it is higher than 2
                }*/
                break;
            // -----------
            
            case Autodrive::PARKING:
                lastCommand = Parking::Park();
                break; 
                
            case Autodrive::UNKNOWN:
                break;
                
            case Autodrive::DEBUG:
                lastCommand = Parking::Debug();
                break;
                
            case Autodrive::DISTANCE_MEASURED_DEBUG:
                lastCommand = Parking::DebugGapLength();
                break;
                
            default:
                break;
        }
    }
}
