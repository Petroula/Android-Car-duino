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

        Autodrive::imageProcessor::continue_processing(frame);

        Autodrive::show_image(frame, 3, "w");
        waitKey(10); // waits to display frame
    }
    return 0;
}
