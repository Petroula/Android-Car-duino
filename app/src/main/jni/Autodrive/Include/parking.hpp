#pragma once
#include <string.h>
#include <math.h>
#include "imageprocessor/command.hpp"
#include "sensordata.hpp"
#include "maneuver.hpp"

namespace Autodrive {

	namespace Parking {
		
		// the different parking procedures
		enum Procedure { NO_PROCEDURE, PARALLEL_STANDARD, PARALLEL_WIDE, PERPENDICULAR_STANDARD };
        Procedure parkingProcedure;
		
		int gapLength = 0;
		int gapStart = 0;
		
		//debugStop
		bool ObstacleFound = false;
		
		// measure the length of a gap
	    void SetGapLength(){
	        if(SensorData::infrared[1] < 1){
				gapLength = SensorData::encoderDistance() - gapStart;
			}else{
				gapStart = SensorData::encoderDistance();
			}
	    }
	
	    // defines the procedure to engage depending on the size of a  gap
		void SetParkingProcedure() {
			//std::cout << gapLength << std::endl;						
	        // return the appropriate parking procedure
			if (SensorData::usFrontRight < 1) {		
				parkingProcedure = PARALLEL_WIDE;
			} else {
				if (gapLength > 100 && SensorData::infrared[1] > 0) {		
					parkingProcedure = PARALLEL_STANDARD;
				} else if (gapLength > 20 && gapLength < 80 /*&& SensorData::infrared[1] > 0*/) {
					//std::cout << "values correct" << std::endl;
					ObstacleFound = true;
					parkingProcedure = PERPENDICULAR_STANDARD;
				} else {
					parkingProcedure = NO_PROCEDURE;
				}
			}
		}
		
		//=====================================================
		command Debug(){
			if(SensorData::infrared[0] > 0 || SensorData::infrared[1] > 0){
				ObstacleFound = true;
				return Maneuver::Stop();
			}else{
				if(!ObstacleFound){
					return Maneuver::Move(Maneuver::slowSpeed);
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
           	//std::cout << "PARKING" << std::endl;
			command cmd; // so ugly!
            
            switch (parkingProcedure) {			                        // switch to the appropriate parking procedure
	            
                case PARALLEL_STANDARD:
					//std::cout << "PARALLEL_STANDARD" << std::endl;						
    				return Maneuver::ParallelStandard();
                    
    			case PARALLEL_WIDE:
					//std::cout << "PARALLEL_WIDE" << std::endl;
    				return cmd; //Maneuver::ParallelWide();
    				
    			case PERPENDICULAR_STANDARD:
    				return Maneuver::PerpendicularStandard();
                    
    			default:
    				return cmd;
            }
	    }													
	} // Parking
}

