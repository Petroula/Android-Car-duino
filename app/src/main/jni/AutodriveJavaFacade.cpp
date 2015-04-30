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
    
    
    TYPE(void) NAME(reset)()
    {
        return Autodrive::reset();
    }

    TYPE(jint) NAME(getTargetAngle)()
    {
        return Autodrive::getAngle();
    }
}

#undef TYPE
#undef NAME
#undef PARAMS
