/**
 * 
 *
 *  - - - - -- - -  Parking , parking logic for Autodrive
 *
 *
 */

#pragma once
#include <string.h>
#include <math.h>
#include "command.hpp"
#include "sensordata.hpp"
#include "maneuver.hpp"

namespace Autodrive {

	namespace Parking {
		
		// the different parking maneuvers
		enum pManeuver { NO_MANEUVER, PARALLEL_STANDARD, PARALLEL_WIDE, PERPENDICULAR_STANDARD };
        pManeuver currentManeuver;
		
		int gapLength = 0;
		int gapStart = 0;
		
		// measure the length of a gap
	    void SetGapLength(){
	        if(SensorData::infrared.rearright < 1){
				gapLength = SensorData::encoderDistance() - gapStart;
			}else{
				gapStart = SensorData::encoderDistance();
			}
	    }
	
	    // the maneuver to engage depending on the size of a gap
		void SetParkingManeuver() {						
			SetGapLength();
			
			// perpendicular standard
			// if the gap length is between half the size of the car and the size of the car
			if (gapLength > (0.5 * SensorData::carLength) && gapLength < (1 * SensorData::carLength) && SensorData::infrared.rearright > 0) {
				currentManeuver = PERPENDICULAR_STANDARD;
			
			// parallel wide
			// if there is enought space for the car to park front
			}else if(SensorData::ultrasound.frontright < 1){
				currentManeuver = PARALLEL_WIDE;
				
			// parallel standard
			// if there is not enought space for the car to park front on
			}else if(gapLength > 1 * SensorData::carLength && SensorData::infrared.rearright > 0){
				currentManeuver = PARALLEL_STANDARD;
				
			// no matching maneuver
			}else{
				currentManeuver = NO_MANEUVER;
			}
		}
		
		// execute the appropriate parking maneuver
		command Park(){
  
            switch (currentManeuver) {			                        
	            
                case PARALLEL_STANDARD:					
    				return Maneuver::ParallelStandard();
                    
    			case PARALLEL_WIDE:
    				return Maneuver::ParallelWide();
    				
    			case PERPENDICULAR_STANDARD:
    				return Maneuver::PerpendicularStandard();
				
				case NO_MANEUVER:
					return command();
                    
    			default:
    				return command();
            }
	    }													
	} // Parking
} // Autodrive

