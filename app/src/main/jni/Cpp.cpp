#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>

using namespace std;
using namespace cv;

extern "C" {
	JNIEXPORT  	jint JNICALL Java_pegasus_bluetootharduino_Natives_Tick(JNIEnv*, jobject);
						  
	JNIEXPORT  	jint JNICALL Java_pegasus_bluetootharduino_Natives_Tick(JNIEnv*, jobject)
	{
		return 1;
	}
}
