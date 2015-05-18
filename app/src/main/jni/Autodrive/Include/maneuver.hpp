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
	const double slowSpeed = 0.2;
	const double normalSpeed = 0.28;
	
	enum side { right, left};
	enum direction { front, back };

	// the different parking maneuvers
	enum pManeuver { NO_MANEUVER, PARALLEL_STANDARD, PARALLEL_WIDE, PERPENDICULAR_STANDARD };
	
	// functions providing info about the status of the car
	namespace Status {
		
		// measuring distance traveled
		bool measuringDistance = false;
		double startPos = 0;

		// measuring angle turned
		bool measuringAngle = false;
		int startAngle = 0;

		// is the car stopped 
		bool IsStoped(){
			if(SensorData::currentSpeed < 1 && SensorData::currentSpeed > -1){
				return true;
			}else{
				return false;
			}
		}
		
		// checks if the car has traveled a specific distance (in ?)
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

			if(abs(currentAngle) > abs(desiredAngle)) {
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
        enum mState { NOT_MOVING, FORWARD, BACKWARD, FORWARD_RIGHT, BACKWARD_RIGHT, FORWARD_LEFT, BACKWARD_LEFT, DONE};
        mState currentState;
		
		// constructor 
		maneuver(pManeuver m){
			type = m;
			currentState = NOT_MOVING;
		}
		
		// returns the appropriate command depending on the current maneuver and its state
		command GetCommand(){
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
		
		mState GetOpositeState(mState m){
			switch(m){
				case FORWARD:
					return BACKWARD;
				case BACKWARD:
					return FORWARD;
				case FORWARD_RIGHT:
					return BACKWARD_LEFT;
				case BACKWARD_LEFT:
					return FORWARD_RIGHT;
				case FORWARD_LEFT:
					return BACKWARD_RIGHT;
				case BACKWARD_RIGHT:
					return FORWARD_LEFT;
				default:
					return NOT_MOVING;
			}
		}
		
		// the procedure for perpendicular parking
		command PerpendicularStandard(){
			command cmd;
			switch(currentState){
				case NOT_MOVING:
					if(Status::IsStoped()){
						currentState = BACKWARD_RIGHT;
						cmd = command();
					}else{
						cmd.setSpeed(0);
					}
				break;
				case BACKWARD_RIGHT:
					cmd.setSpeed(slowSpeed * -1);
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
					if(Status::IsStoped()){
						currentState = BACKWARD_RIGHT;
						cmd = command();
					}else{
						cmd.setSpeed(0);
					}
				break;
				case BACKWARD_RIGHT:
					cmd.setSpeed(slowSpeed * -1);
					if(Status::HasTurnedAngle(80)){
						currentState = BACKWARD_LEFT;
						cmd.setSpeed(0);
					}else{
						cmd.setAngle(1.0);
					}
				break;
				case BACKWARD_LEFT:
					cmd.setSpeed(slowSpeed * -1);
					if(Status::HasTurnedAngle(80)){
						currentState = DONE;
						cmd.setSpeed(0);
					}else{
						cmd.setAngle(-1.0);
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

