#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

//TODO: Only enable when debugging emulator
#define _DEBUG
#include "Autodrive/Include/autodrive.hpp"
#include "Autodrive/Include/sensordata.hpp"
#include "Autodrive/Include/maneuver.hpp"

using namespace std;
using namespace cv;

#define TYPE(x) JNIEXPORT x JNICALL
#define NAME(x) Java_pegasus_bluetootharduino_Autodrive_##x
#define PARAMS(...) (JNIEnv*,jobject , __VA_ARGS__)

extern "C" 
{
    TYPE(void) NAME (drive)()
    {
        Autodrive::drive();
    }
	
    TYPE(void) NAME(reset)()
    {
        return Autodrive::resetStatus();
    }

    TYPE(void) NAME(setParkingMode)()
    {
        Autodrive::setInitialStatus(Autodrive::DETECTING_GAP);
    }
    
    TYPE(void) NAME(resetParking)()
    {
        Autodrive::Parking::Reset();
    }

    TYPE(void) NAME(setLeftLane) PARAMS(bool boolean)
    {
        Autodrive::Status::setLeftLane(boolean);
    }
    
    TYPE(void) NAME(setCarLength) PARAMS(int _carLength)
    {
        Autodrive::SensorData::carLength = _carLength;
    }

    TYPE(void) NAME(lineLeftFound)()
    {
        Autodrive::SensorData::lineLeftFound = true;
    }

    TYPE(void) NAME(lineRightFound)()
    {
        Autodrive::SensorData::lineRightFound = true;
    }
    
    TYPE(jint) NAME(getCarLength)()
    {
        return Autodrive::SensorData::carLength;
    }

   /*----- DEBUGDATA -----*/
   
   // parking

   TYPE(jboolean)NAME(isGapDepthOk)()
   {
        return Autodrive::Parking::gapDepthOk;
   }

   TYPE(jboolean)NAME(isInitialGap)()
   {
        return Autodrive::Parking::initialGap;
   }

   
   TYPE(jint) NAME(gapLength)()
  	{
	    return Autodrive::Parking::gapLength;
	}
    
    TYPE(jint) NAME(angleTurned)()
    {
        return Autodrive::Status::currentAngle;
    }
    
    TYPE(jint)NAME(getManeuver)()
    {
        switch(Autodrive::Parking::currentManeuver.type)
        {
            case Autodrive::NO_MANEUVER:
                return 0;
            case Autodrive::PARALLEL_STANDARD:
                return 1;
            case Autodrive::PARALLEL_WIDE:
                return 2;
            case Autodrive::PERPENDICULAR_STANDARD:
                return 3;
            default:
                return -1;
        }
    }
    
    TYPE(jint)NAME(getManeuverState)()
    {
        switch(Autodrive::Parking::currentManeuver.currentState)
        {
            case Autodrive::maneuver::mState::NOT_MOVING:
                return 0;
            case Autodrive::maneuver::mState::FORWARD:
                return 1;
            case Autodrive::maneuver::mState::BACKWARD:
                return 2;
            case Autodrive::maneuver::mState::FORWARD_RIGHT:
                return 3;
            case Autodrive::maneuver::mState::BACKWARD_RIGHT:
                return 4;
            case Autodrive::maneuver::mState::FORWARD_LEFT:
                return 5;
            case Autodrive::maneuver::mState::BACKWARD_LEFT:
                return 6;
            case Autodrive::maneuver::mState::DONE:
                return 7;
            default:
                return -1;
        }
    }
    
    /*----- SENSORDATA -----*/
    
    // getters - for debuging purposes
    
    TYPE(jint)NAME(usFront)()
    {
        return Autodrive::SensorData::ultrasound.front;
    }
    
    TYPE(jint)NAME(usFrontRight)()
    {
        return Autodrive::SensorData::ultrasound.frontright;
    }
    
    TYPE(jint)NAME(usRear)()
    {
        return Autodrive::SensorData::ultrasound.rear;
    }
    
    TYPE(jint)NAME(irFrontRight)()
    {
        return Autodrive::SensorData::infrared.frontright;
    }
    
    TYPE(jint)NAME(irRearRight)()
    {
        return Autodrive::SensorData::infrared.rearright;
    }
    
    TYPE(jint)NAME(irRear)()
    {
        return Autodrive::SensorData::infrared.rear;
    }
    
    TYPE(jint)NAME(gyroHeading)()
    {
        return Autodrive::SensorData::gyroHeading;
    }
    
    TYPE(jint)NAME(razorHeading)()
    {
        return Autodrive::SensorData::razorHeading;
    }
    
   // setters

    TYPE(void) NAME(setImage) PARAMS(long newMat){
        Autodrive::SensorData::image = (cv::Mat*)newMat;
    }

    TYPE(void) NAME(setUltrasound) PARAMS(int sensor,int value){
        switch (sensor)
        {
            case 0:
                Autodrive::SensorData::ultrasound.front = value;
                break;
            case 1:
                Autodrive::SensorData::ultrasound.frontright = value;
                break;
            default:
            case 2:
                Autodrive::SensorData::ultrasound.rear = value;
                break;
        }
    }

    TYPE(void) NAME(setInfrared) PARAMS(int sensor,int value){
        switch (sensor)
        {
            case 0:
                Autodrive::SensorData::infrared.frontright = value;
                break;
            case 1:
                Autodrive::SensorData::infrared.rearright = value;
                break;
            default:
            case 2:
                Autodrive::SensorData::infrared.rear = value;
                break;
        }

    }

    TYPE(void) NAME(setEncoderPulses) PARAMS(long value){
        Autodrive::SensorData::encoderPulses = value;
    }
    
    TYPE(void) NAME(setGyroHeading) PARAMS(int value){
        Autodrive::SensorData::gyroHeading = value;
    }
    
    TYPE(void) NAME(setRazorHeading) PARAMS(int value){
        Autodrive::SensorData::razorHeading = value;
    }
    
	
	/*----- RESULTING AUTODRIVE DATA -----*/
	
    TYPE(jboolean) NAME(speedChanged)() 
    {
        return Autodrive::speedChanged();
    }

    TYPE(jboolean) NAME(angleChanged)()
    {
        return Autodrive::angleChanged();
    }

    TYPE(jdouble) NAME(getTargetSpeed)()
    {
        return Autodrive::getSpeed();
    }

    TYPE(jdouble) NAME(getTargetAngle)()
    {
        return Autodrive::getAngle();
    }
	
	/* SETTINGS */
	
    TYPE(void) NAME(setSettingLightNormalization) PARAMS(bool on)
    {
        Autodrive::Settings::normalizeLightning = on;
    }
    
    TYPE(void) NAME(setSettingUseLeftLine) PARAMS(bool on)
    {
        Autodrive::Settings::useLeftLine = on;
    }
    
    TYPE(void) NAME(setSettingSmoothening) PARAMS(int value)
    {
        Autodrive::Settings::smoothening = value;
    }

    TYPE(void) NAME(setSettingFirstFragmentMaxDist) PARAMS(int value)
    {
        Autodrive::Settings::firstFragmentMaxDist = value;
    }

    TYPE(void) NAME(setSettingLeftIterationLength) PARAMS(int value)
    {
        Autodrive::Settings::leftIterationLength = value;
    }

    TYPE(void) NAME(setSettingRightIterationLength) PARAMS(int value)
    {
        Autodrive::Settings::rightIterationLength = value;
    }

    TYPE(void) NAME(setSettingMaxAngleDiff) PARAMS(float value)
    {
        Autodrive::Settings::maxAngleDiff = value;
    }

    TYPE(void) NAME(setPIDkp) PARAMS(float value)
    {
        Autodrive::Settings::kp = value;
    }

    TYPE(void) NAME(setPIDki) PARAMS(float value)
    {
        Autodrive::Settings::ki = value;
    }

    TYPE(void) NAME(setPIDkd) PARAMS(float value)
    {
        Autodrive::Settings::kd = value;
    }
}

#undef TYPE
#undef NAME
#undef PARAMS
