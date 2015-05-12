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
    TYPE(void) NAME (drive) ()
    {
        Autodrive::drive();
    }
	
    TYPE(void) NAME(reset)()
    {
        return Autodrive::reset();
    }

//   /*----- DEBUGDATA -----*/
//
//   TYPE(jint) NAME(getGapLength)()
//  	{
//	    return Autodrive::Parking::gapLength;
//	}
    
	/*----- SENSORDATA -----*/

    TYPE(void) NAME(setImage) PARAMS(long newMat){
        Autodrive::SensorData::image = (cv::Mat*)newMat;
    }

    TYPE(void) NAME(setUltrasound) PARAMS(int sensor,int value){
        Autodrive::SensorData::ultrasound[sensor] = value;

    }

    TYPE(void) NAME(setInfrared) PARAMS(int sensor,int value){
        Autodrive::SensorData::infrared[sensor] = value;
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

    TYPE(jint) NAME(getTargetSpeed)()
    {
        return Autodrive::getSpeed();
    }

    TYPE(jint) NAME(getTargetAngle)()
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
