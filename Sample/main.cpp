#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#define _AUTODRIVE_SHOWCANNY
#define _AUTODRIVE_SHOWHOUGH

#undef _DEBUG
#include "../Include/imageprocessor.hpp"

using namespace cv;
using namespace std;

int main()
{
    //string filename = "testreal_small_portrait.mp4";
    string filename = "testreal_small.mp4";
    //string filename = "Test4-1.m4v";
    VideoCapture capture(filename);
    Mat frame;
    float angle = 0;
    float speed = 5;

    if (!capture.isOpened())
    throw "Error when opening test4.avi";
    string window = "w";
    namedWindow(window, 1);
    int birds_angle = 34;
    int crop = 0;//20
    int thresh1 = 23;
    int thresh2 = 38;
    /*createTrackbar("Birds Eye angle:", window, &birds_angle, 100);
    createTrackbar("Crop:", window, &crop, 50);
    createTrackbar("Thresh1:", window, &thresh1, 50);
    createTrackbar("Thresh2:", window, &thresh2, 50);
    */
    capture >> frame;
    while (!Autodrive::init_processing(frame)){

        capture >> frame;
        /*cv::Mat resized;
        cv::resize(frame, resized, frame.size() * 3);//resize image
        cv::putText(resized, "SEARCHING FOR STRAIGHT LANES...", cv::Point2f(50.f, resized.size().height / 2.f),FONT_HERSHEY_PLAIN,3,cv::Scalar(0,255,0),2);
        cv::imshow("mainwindow", resized);
        cv::waitKey(5); // waits to display frame*/
    }
    for (;;)
    {
        capture >> frame;
        if (frame.empty()){
            capture.open(filename);
            continue;
        }

        Autodrive::command cmnd = Autodrive::continue_processing(frame);


        if (cmnd.changedAngle){
            angle = cmnd.angle;
        }

        if (cmnd.changedSpeed){
            speed = cmnd.speed;
        }

        Point2f center(frame.size().width / 2.f, (float) frame.size().height);
        
        Autodrive::linef(center, center + Point2f(cos(angle) * 200, -sin(angle) * 200)).draw(frame, CV_RGB(0, 250, 0));

        imshow("w", frame);
        waitKey(10000); // waits to display frame
    }
    return 0;
}