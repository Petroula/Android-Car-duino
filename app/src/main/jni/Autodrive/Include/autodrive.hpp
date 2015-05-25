#pragma once
#include "sensordata.hpp"
#include "parking.hpp"
#include "imageprocessor/imageprocessor.hpp"
#include "overtaking.hpp"

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
        DETECTING_GAP, PARKING, SEARCHING_FOR_LANES, FOLLOWING_LANES, UNKNOWN
    };  
    
    carstatus initialStatus = SEARCHING_FOR_LANES;
    carstatus status = initialStatus;

    void setInitialStatus(carstatus newStatus)
    {
        initialStatus = newStatus;
        status = newStatus;
    }

    void resetStatus()
    {
       status = initialStatus;
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
                    lastCommand.setSpeed(normalSpeed);
                    status = FOLLOWING_LANES;
                }
                break;
                
            case Autodrive::FOLLOWING_LANES:
                lastCommand = Autodrive::imageProcessor::continue_processing(*Autodrive::SensorData::image);
                lastCommand = Overtaking::run(lastCommand);
                break;
                
            // debug only! will be merged with lane following   
            case Autodrive::DETECTING_GAP:
                Parking::SetParkingManeuver(); // check what parking maneuver to initialize, if any
                
                if(Parking::currentManeuver.type != NO_MANEUVER){
                    status = PARKING;
                }else{
                    lastCommand.setSpeed(normalSpeed); 
                }
                break;
            // -----------
            
            case Autodrive::PARKING:
                lastCommand = Parking::Park();
                if(Parking::currentManeuver.currentState == Autodrive::maneuver::mState::DONE){
                    Parking::currentManeuver.type = NO_MANEUVER;
                }
                break; 
                
            case Autodrive::UNKNOWN:
                break;
                
            default:
                break;
        }
    }
}
