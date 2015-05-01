#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#define _AUTODRIVE_SHOWCANNY
#define _AUTODRIVE_SHOWHOUGH

#undef _DEBUG
#include "../Include/imageprocessor/imageprocessor.hpp"

using namespace cv;
using namespace std;

int main()
{
std::cout<<"main";
    string filename = "testreal_small.mp4";
    //string filename = "vid1.mp4";
    //string filename = "Test4-1.m4v";
    VideoCapture capture(filename);
    Mat frame;
    float angle = 0;

    if (!capture.isOpened())
    throw "Error when opening test4.avi";
    string window = "w";
    namedWindow(window, 1);

    capture >> frame;
    while (!Autodrive::imageProcessor::init_processing(&frame)){
        capture >> frame;
    }
    for (;;)
    {
        capture >> frame;
        if (frame.empty()){
            capture.open(filename);
            continue;
        }

        Autodrive::command cmnd = Autodrive::imageProcessor::continue_processing(frame);

        if (cmnd.changedAngle){
            angle = ((90 - cmnd.angle)* Autodrive::Mathf::PI) / 180.f ;
        }

        Point2f center(frame.size().width / 2.f, (float) frame.size().height);
        
        Autodrive::linef(center, center + Point2f(cos(angle) * 200, -sin(angle) * 200)).draw(frame, CV_RGB(0, 250, 0));

        Autodrive::show_image(frame, 3, "w");
        waitKey(10); // waits to display frame
    }
    return 0;
}
