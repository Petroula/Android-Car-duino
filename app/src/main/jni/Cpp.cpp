#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>


//TODO: Only enable when debugging emulator
#define _DEBUG
#include "Autodrive/Include/imageprocessor.hpp"

using namespace std;
using namespace cv;

extern "C" {
    
    JNIEXPORT jfloat JNICALL Java_pegasus_bluetootharduino_Natives_processImage(JNIEnv*, jobject,jlong addrRgba);
						  
	JNIEXPORT jfloat JNICALL Java_pegasus_bluetootharduino_Natives_processImage(JNIEnv*, jobject,jlong addrRgba)
	{
        Mat& mRgb = *(Mat*)addrRgba;
        Autodrive::command cmnd = Autodrive::processImage(mRgb,mRgb.step);
        return cmnd.angle;
	}
}
