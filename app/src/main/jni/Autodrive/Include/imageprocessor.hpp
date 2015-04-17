#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "command.hpp"
#include "sensordata.h"

using namespace std;


namespace Autodrive {

    command processImage(cv::Mat& mat, int widthStep);

    int intersection_protect = 0;

    command drive()
    {
        return processImage(*Autodrive::SensorData::image,Autodrive::SensorData::image->step );
    }

    // You should start your work in this method.
    command processImage(cv::Mat& mat, int widthStep) {
        int width = mat.size().width;
        int height = mat.size().height;
        int step = widthStep;
        unsigned char* image = mat.data;

        int sample_near = 60; // define near vision
        int sample_mid = 150; //define mid vision
        int sample_mid2 = 90; //define mid vision
        int sample_mid3 = 120; //define mid vision
        int sample_far = 240; // define further vision
        int desired_right_near = 241; //243
        int desired_right_far = 75; // 73
        double k = 0.12; //portion control 0.2
        int max_left = -24;
        int max_right = 24;// max turning steering
        //cout << " width:"<< width<<endl;
        //cout << " height:"<< height<<endl;
        CvScalar red = CV_RGB(250, 0, 0);
        CvScalar green = CV_RGB(0, 250, 0);
        CvScalar blue = CV_RGB(0, 0, 250);
        int thickness = 1;
        int connectivity = 8;
        bool near_lost = 0;// near vision lost
        bool far_lost = 0;//  far vision lost
        bool left_lost = 0; // left vision lost (left)


        double left_range = 0.4; // (0,0.5)

        CvPoint ver_centr_start = cvPoint(width / 2, height);
        CvPoint ver_centr_end = cvPoint(width / 2, 0);
        CvPoint near_sample_start = cvPoint(width / 2, height - sample_near);
        CvPoint mid_sample_start = cvPoint(width / 2, height - sample_mid);
        CvPoint mid2_sample_start = cvPoint(width / 2, height - sample_mid2);
        CvPoint mid3_sample_start = cvPoint(width / 2, height - sample_mid3);
        CvPoint near_sample_end;
        CvPoint mid_sample_left_end;
        CvPoint mid2_sample_left_end;
        CvPoint mid3_sample_left_end;
        CvPoint near_sample_left_end;
        CvPoint far_sample_start = cvPoint(width / 2, height - sample_far);
        CvPoint far_sample_end;

        //TODO: Start here.
        int right_near = 0;
        int right_near1 = 0;
        int right_near2 = 0;
        int right_far = 0;
        int left_near = 0;
        int left_mid = 0;
        int left_mid2 = 0;
        int left_mid3 = 0;
        // 1. Do something with the image m_image here, for example: find lane marking features, optimize quality, ...
        // find right distance
        //  I (x, y) ~ ((unsigned char*) (img-> imageData + img-> widthStep * y)) [x]       for 3 chanel Iplimage
        // I.at<uchar>(i,j) get value for Mat

        const float threshold = 200;

        while ((image + step * (height - sample_near - 2))[(width / 2 + right_near1) * 3] < threshold && right_near1 < width / 2){ right_near1++; }
        while ((image + step * (height - sample_near + 2))[(width / 2 + right_near2) * 3] < threshold && right_near2 < width / 2){ right_near2++; }
        while ((image + step * (height - sample_far))[(width / 2 + right_far) * 3] < threshold && right_far < width / 2) { right_far++; }
        while ((image + step * (height - sample_near))[(width / 2 - left_near) * 3] < threshold && left_near < width / 2){ left_near++; }
        while ((image + step * (height - sample_mid))[(width / 2 - left_mid) * 3] < threshold && left_mid < width / 2){ left_mid++; }
        while ((image + step * (height - sample_mid2))[(width / 2 - left_mid2) * 3] < threshold && left_mid2 < width / 2){ left_mid2++; }
        while ((image + step * (height - sample_mid3))[(width / 2 - left_mid3) * 3] < threshold && left_mid3 < width / 2){ left_mid3++; }


        if (right_near1 == 0 || right_near2 == 0){ // this case happens only when running into the stop line when entering the intersection
            right_near = desired_right_near;
        }

        if (right_near1 >= width / 2 && right_near2 >= width / 2){ // near vision lost
            near_lost = 1;

        }
        else { 		// near vision not lost , decide right_near
            if (right_near1 >= width / 2){
                right_near = right_near2;
            }
            else if (right_near2 >= width / 2){
                right_near = right_near1;
            }
            else{
                right_near = (right_near1 + right_near2) / 2;
            }
            near_lost = 0;
        }

        if (right_far >= width / 4){ // far vision lost
            far_lost = 1;
        }
        else {
            far_lost = 0;
        }

        if (left_near >= width*left_range && left_mid >= width*left_range && left_mid2 >= width*left_range && left_mid3 >= width*left_range){ // left lost left
            left_lost = 1;
        }
        else {
            left_lost = 0;
        }

#ifdef _DEBUGIMAGEPROCESSING

        // state machine

        if (near_lost){
            cout << "near lost" << endl;
        }
        else{
            cout << "right_near:" << right_near << endl;
        }

        if (far_lost){
            cout << "far lost" << endl;
        }
        else{
            cout << "right_far:" << right_far << endl;
        }
#endif

#ifdef _DEBUG
        near_sample_end = cvPoint(width / 2 + right_near, height - sample_near);
        far_sample_end = cvPoint(width / 2 + right_far, height - sample_far);
        near_sample_left_end = cvPoint(width / 2 - left_near, height - sample_near);
        mid_sample_left_end = cvPoint(width / 2 - left_mid, height - sample_mid);
        mid2_sample_left_end = cvPoint(width / 2 - left_mid2, height - sample_mid2);
        mid3_sample_left_end = cvPoint(width / 2 - left_mid3, height - sample_mid3);
        if (mat.data != NULL) {
            cv::line(mat, ver_centr_start, ver_centr_end, red, thickness, connectivity);
            cv::line(mat, near_sample_start, near_sample_end, green, thickness, connectivity);
            cv::line(mat, far_sample_start, far_sample_end, green, thickness, connectivity);
            cv::line(mat, near_sample_start, near_sample_left_end, blue, thickness, connectivity);
            cv::line(mat, mid_sample_start, mid_sample_left_end, blue, thickness, connectivity);
            cv::line(mat, mid2_sample_start, mid2_sample_left_end, blue, thickness, connectivity);
            cv::line(mat, mid3_sample_start, mid3_sample_left_end, blue, thickness, connectivity);
        }
#endif

        // 2. Calculate desired steering commands from your image features to be processed by driver.
        double difference = 0;
        if (!near_lost){
            difference = (right_near - desired_right_near) *k;
            intersection_protect = 0; // unflag intersection protects

        }
        else{   	 // near lost
            if (!left_lost && !intersection_protect){
                difference = max_right;

            }
            else if (!far_lost){ //intersection mode
                difference = (right_far - desired_right_far) *k;
                intersection_protect = 1;//
            }
        }

        if (difference < max_left) difference = max_left;
        if (difference > max_right) difference = max_right;

        command carCommand;
        carCommand.setAngle(difference);
        return carCommand;
    }
}
