#pragma once
#include <string.h>
#include <math.h>
#include "command.hpp"
#include "sensordata.hpp"

namespace Autodrive {

	namespace Maneuver{
		
		int mInt = 0;

		double slowSpeed = 0.16;
		double normalSpeed = 0.28;

		// measuring distance traveled
		bool measuringDistance = false;
		double startPos = 0;

		// measuring angle turned
		bool measuringAngle = false;
		int startAngle = 0;

		enum direction { right, left, front, back };

		// maneuvers
        enum Maneuver { NO_MANEUVER, FORWARDS, BACKWARDS, FORWARD_RIGHT, BACKWARDS_RIGHT, FORWARD_LEFT, BACKWARDS_LEFT, DONE };
        Maneuver currentManeuver;
		
		//added flag
		bool init = false;

		// stops the car
		command Stop() {
			command cmd;
			cmd.setSpeed(0);
			cmd.setAngle(0);
			std::cout << "STOPPING" << std::endl;
			return cmd;
		}

		// sets the car speed
		command SetSpeed(double speed) {
			command cmd;
			cmd.setSpeed(speed);
			return cmd;
		}

		// moves the car
		command Move(int speed) {
			command cmd;
			cmd.setSpeed(speed);
			cmd.setAngle(0);
			return cmd;
		}

		// turns the car
		command Turn(double angle) {
			command cmd;
			cmd.setSpeed(slowSpeed);
			cmd.setAngle(angle);
			return cmd;
		}

		command Turn(direction direction) {
			command cmd;
			if (direction == right) {
				cmd.setAngle(1.0);
			} else {
				cmd.setAngle(-1.0);
			}
			return cmd;
		}
		
		command Turn(command cmd, direction direction) {
			if (direction == right) {
				cmd.setAngle(1.0);
			} else {
				cmd.setAngle(-1.0);
			}
			return cmd;
		}

		namespace Status {

			// is the car stopped
			bool IsStoped(){
				if(SensorData::currentSpeed < 1 && SensorData::currentSpeed > -1){
					return true;
				}else{
					return false;
				}
			}

			// checks if the car has traveled a specific distance
			bool HasTraveledDistance(int distance){

				// initialize start point
				if(!measuringDistance){
					startPos = SensorData::encoderDistance();
					measuringDistance = true;
				}

				if((SensorData::encoderDistance() - startPos) > distance){
					measuringDistance = false;
					return true;
				}else{
					return false;
				}
			}

			// checks if the car has turned a specific angle
			bool HasTurnedAngle(int desiredAngle){

				float currentAngle = 0;

				// initialize start point
				if(!measuringAngle){
					startAngle = SensorData::gyroHeading;
					measuringAngle = true;
				}

				// get the current angle from where the car was, to where it is now
				currentAngle = fmod((startAngle - SensorData::gyroHeading),360);
				if(currentAngle > 180) currentAngle = 360 - currentAngle;

				if(abs(currentAngle) > abs(desiredAngle)) { // TODO make sure currentAngle & desiredAngle are INT
					measuringAngle = false;
					return true;
				}else{
					return false;
				}
			}

			// stops the car if it gets too close to something, provided a direction to pay attention too
			bool EmergencyStop(direction direction){
				if(SensorData::infrared.rear > -1 && SensorData::infrared.rear < 2.5 && direction == back){
					return true;
				}else if(((SensorData::ultrasound.front > -1 && SensorData::ultrasound.front < 2) || (SensorData::ultrasound.frontright > -1 && SensorData::ultrasound.frontright <2)) && direction == front){
					return true;
		        }else{
		        	return false;
		        }
			}
		}

		/* ------------------------------------------ */
		/* the parallel parking maneuver */
		command ParallelStandard(){
			
			command empty;

			switch(currentManeuver){

				case NO_MANEUVER:
					if(Status::IsStoped()){
						currentManeuver = BACKWARDS_RIGHT;
						return empty;
					}else{
						return Stop();
					}

				case BACKWARDS_RIGHT:
					SetSpeed(slowSpeed * -1);

					if(Status::EmergencyStop(back)){
						currentManeuver = FORWARD_RIGHT;
						return Stop();
					}else{
						if(Status::HasTurnedAngle(60)){
							currentManeuver = BACKWARDS_LEFT;
							return empty;
						}else{
							return Turn(right);
						}
					}

				case BACKWARDS_LEFT:
					SetSpeed(slowSpeed * -1);
					if(Status::EmergencyStop(back)){
						currentManeuver = FORWARD_RIGHT;
						return Stop();
					}else{
						if(Status::HasTurnedAngle(-60)){
							currentManeuver = DONE;
							return empty;
						}else{
							return Turn(left);
						}
					}

				case FORWARD_RIGHT:
					SetSpeed(slowSpeed);
					if(Status::EmergencyStop(back)){
						currentManeuver = BACKWARDS_LEFT;
						return Stop();
					}else{
						if(Status::HasTurnedAngle(60)){
							currentManeuver = DONE;
							return empty;
						}else{
							return Turn(right);
						}
					}

				default:
					return empty;
			}
		}
		/* ------------------------------------------ */
		/* the parallel parking maneuver on wide spot */
		command ParallelWide(){
			
			return command();
			
//			switch(currentManeuver){
//				case NO_MANEUVER:
//					cout << "parallel wide" << endl;
//	                if(TryStop()){
//	                    currentManeuver = FORWARD_RIGHT;
//	                }
//					return PARKING;
//	
//	            case FORWARD_RIGHT:
//	                cout << "forward right" << endl;
//	                SetSpeed(turningSpeed);
//	
//	                if(TryTurn(60)){
//	                	currentManeuver = FORWARD_LEFT;
//	                }
//	                return PARKING;
//	
//	            case FORWARD_LEFT:
//	                 cout << "forward left" << endl;
//	                 SetSpeed(turningSpeed);
//	
//	                 if(TryTurn(-60)){
//	                	 if(TryStop()){
//	                		 return DONE;
//	                	 }else{
//	                		 return PARKING;
//	                	 }
//	                 }else{
//	                	 return PARKING;
//	                 }
//	
//				default:
//					return empty;
//			}
		} 
		/* ------------------------------------------ */
		// perpendicular parking maneuver
		command PerpendicularStandard(){
			command cmd;
			//std::cout << "PERPENDICULAR_STANDARD" << std::endl;
			
			if(!init){
				currentManeuver = NO_MANEUVER;
				init = true;
			}
			
			switch(currentManeuver){
				
				case NO_MANEUVER:
					//std::cout << "NO_MANEUVER" << std::endl;
					if(Status::IsStoped()){
						currentManeuver = BACKWARDS_RIGHT;
						return command();
					}else{
						return Stop();
					}
	
				case BACKWARDS_RIGHT:
					//std::cout << "BACKWARDS_RIGHT" << std::endl;
					cmd.setSpeed(slowSpeed * -1);
					
					if(Status::HasTurnedAngle(90)){
						currentManeuver = DONE;
						return Stop();
					}else{
						return Turn(cmd, right);
					}
					// return cmd;
					/*if(Status::EmergencyStop(back)){
						currentManeuver = FORWARD_RIGHT;
						return Stop();
					}else{
						
					}*/
	
	            case BACKWARDS:
					//std::cout << "BACKWARDS" << std::endl;
	                if(Status::HasTraveledDistance(1)){
						currentManeuver = DONE;
						return Stop();
	                }else{
						return Move(slowSpeed);
					}
	
				default:
					//std::cout << "DONE" << std::endl;
					return command();
			}
		}
	} // Maneuver
}

