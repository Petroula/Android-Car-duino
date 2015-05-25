/**
 * 
 *
 *  - - - - -- - -  Maneuver , high level maneuvers for Autodrive
 *
 *
 */

#pragma once
#include <string.h>
#include <math.h>
#include "command.hpp"
#include "sensordata.hpp"

namespace Autodrive {

	// TODO: put in appropriate namespace
	const double slowSpeed = 0.23;
	const double normalSpeed = 0.26;
	const double backwardsSpeed = -0.7;

	enum side { right, left};
	enum direction { front, back };

	// the different parking maneuvers
	enum pManeuver { NO_MANEUVER, PARALLEL_STANDARD, PARALLEL_WIDE, PERPENDICULAR_STANDARD };
	
	// functions providing info about the status of the car
	namespace Status {

		// measuring distance traveled
		bool measuringDistance = false;
		double startPos = 0;

		bool isLeftLane = false;

		// measuring angle turned
		bool measuringAngle = false;
		int startAngle = 0;

		int currentAngle = 0;
		int remainingAngle = 0;

		void setLeftLane(bool boolean) {
			isLeftLane = boolean;
		}

		// is the car stopped 
		bool IsStoped(){
			if(SensorData::currentSpeed < 1 && SensorData::currentSpeed > -1){
				return true;
			}else{
				return false;
			}
		}
		
		// checks if the car has traveled a specific distance (in cm?)
		bool HasTraveledDistance(float distance){

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

			// initialize start point
			if(!measuringAngle){
				startAngle = SensorData::gyroHeading;
				//remainingAngle = 0;
				measuringAngle = true;
			}

			// get the current angle from where the car was, to where it is now
			currentAngle = (startAngle - SensorData::gyroHeading) % 360;
			if(currentAngle > 180) currentAngle = 360 - currentAngle;

			if(abs(currentAngle) >= abs(desiredAngle) || HasTraveledDistance(desiredAngle * 1.55 )) {
				measuringAngle = false;
				return true;
			}else{
				return false;
			}
		}
	} // status

	struct maneuver{
		
		// the selected maneuver
		pManeuver type;
		
		// the different states of the maneuver
        enum mState { NOT_MOVING, FORWARD, BACKWARD, FORWARD_RIGHT, BACKWARD_RIGHT, FORWARD_LEFT, BACKWARD_LEFT, DONE, EMERGENCY};
        mState currentState;
		
		// constructor 
		maneuver(pManeuver m){
			type = m;
			currentState = NOT_MOVING;
		}
		
		// returns the appropriate command depending on the current maneuver and its state
		command GetCommand(){
			if(currentState == DONE){
				type = NO_MANEUVER;
			}
			switch (type) {			                        
                case PARALLEL_STANDARD:					
    				return ParallelStandard();
    			case PARALLEL_WIDE:
    				return ParallelWide();
    			case PERPENDICULAR_STANDARD:
    				return PerpendicularStandard();
				case NO_MANEUVER:
					return command();
    			default:
    				return command();
            }
		}
		
		// the procedure for perpendicular parking
		command PerpendicularStandard(){
			command cmd;
			switch(currentState){
				case NOT_MOVING:
					if(Status::HasTraveledDistance(0.25*SensorData::carLength)){
						currentState = BACKWARD_RIGHT;
						cmd = command();
					}else{
						cmd.setSpeed(slowSpeed);
					}
				break;
				case BACKWARD_RIGHT:
					cmd.setSpeed(backwardsSpeed);
					if(Status::HasTurnedAngle(80)){
						currentState = DONE;
						cmd.setSpeed(0);
					}else{
						cmd.setAngle(1.0);
					}
				break;
				default:
					cmd = command();
			}
			return cmd;
		}
		
		// the procedure for parallel parking
		command ParallelStandard(){
			command cmd;
			
			switch(currentState){
				case NOT_MOVING:
					if(Status::HasTraveledDistance(0.1*SensorData::carLength)){
						currentState = BACKWARD_RIGHT;
						cmd = command();
					}else{
						cmd.setSpeed(slowSpeed);
					}
				break;
				case BACKWARD_RIGHT:
					if(Status::HasTurnedAngle(50)){
						currentState = BACKWARD_LEFT;
						cmd.setSpeed(0);
					}else{
						cmd.setAngle(1.0);
						cmd.setSpeed(backwardsSpeed);
					}
				break;
				case BACKWARD_LEFT:
					if(Status::HasTurnedAngle(45)){
						currentState = DONE;
						cmd.setSpeed(0);
					}else{
						cmd.setAngle(-1.0);
						cmd.setSpeed(backwardsSpeed);
					}
					if(SensorData::infrared.rear > 0 || (SensorData::ultrasound.rear > 1 && SensorData::ultrasound.rear < 40)){	// TODO emergency stop maneuver
						cmd.setSpeed(normalSpeed);
						
						Status::remainingAngle = 20 - Status::currentAngle; // err if the car turns in several directions
						Status::measuringAngle = false;
						Status::measuringDistance = false;
						currentState = FORWARD_RIGHT;
					}
				break;
				case FORWARD_RIGHT:
					/*if(Status::HasTurnedAngle(Status::remainingAngle)){
						currentState = DONE;
						cmd.setSpeed(0);
					}else{
						cmd.setSpeed(slowSpeed);
						cmd.setAngle(1);
					}*/
					
					if((SensorData::ultrasound.front > 0 && SensorData::ultrasound.front < 25)){	// TODO emergency stop maneuver
						cmd.setSpeed(0);
						currentState = DONE;
						Status::measuringDistance = false;
					}else{
						cmd.setSpeed(slowSpeed);
						cmd.setAngle(0);
					}
				break;
				case BACKWARD:
					if(Status::HasTraveledDistance(0.25*SensorData::carLength) || (SensorData::infrared.rear > 0 && SensorData::infrared.rear < 3)){
						currentState = DONE;
						cmd.setSpeed(0);
					}else{
						cmd.setSpeed(backwardsSpeed);
						cmd.setAngle(0);
					}
				break;
				default:
					cmd = command();
			}
			return cmd;
		}
		
		// extra procedure for parallel parking
		command ParallelWide(){
			command cmd;
			switch(currentState){
				case NOT_MOVING:
					if(Status::IsStoped()){
						currentState = FORWARD_RIGHT;
						cmd = command();
					}else{
						cmd.setSpeed(0);
					}
				break;
				case FORWARD_RIGHT:
					if(Status::HasTurnedAngle(60)){
						currentState = FORWARD_LEFT;
						cmd.setSpeed(0);
					}else{
						cmd.setSpeed(slowSpeed);
						cmd.setAngle(1.0);
					}
				break;
				case FORWARD_LEFT:
					if(Status::HasTurnedAngle(60)){
						currentState = DONE;
						cmd.setSpeed(0);
						cmd.setAngle(0);
					}else{
						cmd.setSpeed(slowSpeed);
						cmd.setAngle(-1.0);
					}
				break;
				default:
					cmd = command();
			}
			return cmd;
		}
	};
}

