#pragma once
#include <string.h>
#include <math.h>
#include "command.hpp"
#include "sensordata.hpp"
#include "maneuver.hpp"

namespace Autodrive {

	namespace Parking {
		
		// the different parking procedures
		enum Procedure { NO_PROCEDURE, PARALLEL_STANDARD, PARALLEL_WIDE, PERPENDICULAR_STANDARD };
        Procedure parkingProcedure;
		
		int gapLength = 0;
		int gapStart = 0;
		
		/* Debug */
		bool ObstacleFound = false;
		bool GapFound = false;
		
		// measure the length of a gap
	    void SetGapLength(){
	        if(SensorData::infrared.rearright < 1){
				gapLength = SensorData::encoderDistance() - gapStart;
			}else{
				gapStart = SensorData::encoderDistance();
			}
	    }
	
	    // defines the procedure to engage depending on the size of a  gap
		command SetParkingProcedure() {						
			SetGapLength();
			/*if (SensorData::usFrontRight < 1) {		
				parkingProcedure = PARALLEL_WIDE;
			} else {
				if (gapLength > 100 * Autodrive::carRatio && SensorData::infrared.rearright > 0) {		
					parkingProcedure = PARALLEL_STANDARD;
				} else if (gapLength > 20 && gapLength < 80 && SensorData::infrared.rearright > 0) {
					GapFound = true;
					parkingProcedure = PERPENDICULAR_STANDARD;
				} else {
					parkingProcedure = NO_PROCEDURE;
				}
			}*/
			
			if (gapLength > (0.5 * SensorData::carLength) && gapLength < (1 * SensorData::carLength) && SensorData::infrared.rearright > 0) {
				GapFound = true;
				parkingProcedure = PERPENDICULAR_STANDARD;
				return Maneuver::Stop();
			}else{
				return Maneuver::Move(Maneuver::normalSpeed);
			}
		}
		
		//=====================================================
		command Debug(){
			if(SensorData::infrared.frontright > 0 || SensorData::infrared.rearright > 0){
				ObstacleFound = true;
				return Maneuver::Stop();
			}else{
				if(!ObstacleFound){
					return Maneuver::Move(Maneuver::slowSpeed * -1);
				}else{
					return Maneuver::Stop();
				}
			}
		}
		
		command DebugGapLength(){
			SetGapLength();
			if(gapLength > 50){
				return Maneuver::Stop();
			}else{
				return Maneuver::Move(Maneuver::normalSpeed);
			}
		}
		//=====================================================
		
		command Park(){
            // switch to the appropriate parking procedure
            switch (parkingProcedure) {			                        
	            
                case PARALLEL_STANDARD:
					//std::cout << "PARALLEL_STANDARD" << std::endl;						
    				return Maneuver::ParallelStandard();
                    
    			case PARALLEL_WIDE:
					//std::cout << "PARALLEL_WIDE" << std::endl;
    				return command(); //Maneuver::ParallelWide();
    				
    			case PERPENDICULAR_STANDARD:
					//std::cout << "PERPENDICULAR_STANDARD" << std::endl;
    				return Maneuver::PerpendicularStandard();
                    
    			default:
    				return command();
            }
	    }													
	} // Parking
}

