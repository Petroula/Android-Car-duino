#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

//TODO: Only enable when debugging emulator
#define _DEBUG
#include "Autodrive/Include/autodrive.hpp"
#include "Autodrive/Include/sensordata.hpp"

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

   /*----- DEBUGDATA -----*/

   TYPE(jint) NAME(getGapLength)()
  	{
	    return Autodrive::Parking::gapLength;
	}
    
    TYPE(jboolean)NAME(hasFoundObstacle)()
    {
        return Autodrive::Parking::ObstacleFound;
    }
    
    TYPE(jboolean)NAME(gapDetected)()
    {
        return Autodrive::Parking::GapFound;
    }
    
    TYPE(jint)NAME(irFrontAutodrive)()
    {
        return Autodrive::SensorData::infrared.frontright;
    }
    
    TYPE(jint)NAME(usFrontAutodrive)()
    {
        return Autodrive::SensorData::ultrasound.front;
    }
    
    TYPE(jint)NAME(getManeuver)(){
        return Autodrive::Maneuver::mInt;
    }
    
	/*----- SENSORDATA -----*/

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
