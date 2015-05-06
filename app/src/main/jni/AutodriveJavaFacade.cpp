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
	
	/*----- SENSORDATA -----*/

    TYPE(void) NAME(setImage) PARAMS(long newMat){
        Autodrive::SensorData::image = (cv::Mat*)newMat;
    }

    TYPE(void) NAME(setUltraSound) PARAMS(int sensor,int value){
        Autodrive::SensorData::ultrasound[sensor] = value;

    }

    TYPE(void) NAME(setInfrared) PARAMS(int sensor,int value){
        Autodrive::SensorData::infrared[sensor] = value;
    }

    TYPE(jint) NAME(getUltraSound) PARAMS(int sensor){
        return Autodrive::SensorData::ultrasound[sensor];
    }
    
    TYPE(jint) NAME(getInfrared) PARAMS(int sensor){
        return Autodrive::SensorData::infrared[sensor];
    }
    
    TYPE(void) NAME(setEncoderPulses) PARAMS(int value){
        Autodrive::SensorData::encoderPulses = value;
    }
    
    TYPE(jdouble) NAME(getEncoderDistance) (){
        return Autodrive::SensorData::encoderDistance();
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
	
    TYPE(void) NAME(setSettingLightNormalization)(bool on)
    {
        Autodrive::Settings::normalizeLightning = on;
    }
    
    TYPE(void) NAME(setSettingUseLeftLine)(bool on)
    {
        Autodrive::Settings::useLeftLine = on;
    }
    
    TYPE(void) NAME(setSettingSmoothening)(int value)
    {
        Autodrive::Settings::smoothening = value;
    }

    TYPE(void) NAME(setSettingFirstFragmentMaxDist)(int value)
    {
        Autodrive::Settings::firstFragmentMaxDist = value;
    }

    TYPE(void) NAME(setSettingLeftIterationLength)(int value)
    {
        Autodrive::Settings::leftIterationLength = value;
    }

    TYPE(void) NAME(setSettingRightIterationLength)(int value)
    {
        Autodrive::Settings::rightIterationLength = value;
    }

    TYPE(void) NAME(setSettingMaxAngleDiff)(float value)
    {
        Autodrive::Settings::maxAngleDiff = value;
    }

    TYPE(void) NAME(setPIDkp)(float value)
    {
        Autodrive::Settings::kp = value;
    }

    TYPE(void) NAME(setPIDki)(float value)
    {
        Autodrive::Settings::ki = value;
    }

    TYPE(void) NAME(setPIDkd)(float value)
    {
        Autodrive::Settings::kd = value;
    }
}

#undef TYPE
#undef NAME
#undef PARAMS
