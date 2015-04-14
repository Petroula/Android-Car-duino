#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "../Include/imageprocessor.hpp"
#define _USE_MATH_DEFINES
#include <math.h>


using namespace cv;
using namespace std;

int main()
{
    string filename = "testdrive.mp4";
    VideoCapture capture(filename);
    Mat frame;
    double angle = 0;
    double speed = 5;

    if (!capture.isOpened())
    throw "Error when opening test4.avi";

    namedWindow("w", 1);
    for (;;)
    {
        capture >> frame;
        if (frame.empty()){
            capture.open(filename);
            continue;
        }

        Autodrive::command cmnd = Autodrive::processImage(frame, frame.step);

        if (cmnd.changedAngle){
            angle = cmnd.angle + 17;
        }

        if (cmnd.changedSpeed){
            speed = cmnd.speed;
        }

        int x1 = frame.size().width / 2;
        int y1 = frame.size().height;

        
        int x2 = x1 + cos(-M_PI_2 + (angle / 10.)*M_PI_2)*speed*20;
        int y2 = y1 + sin(-M_PI_2 + (angle / 10.)*M_PI_2)*speed*20;

        std::cout << "Angle: " << angle << std::endl;

        cv::line(frame, cv::Point2f(x1, y1), cv::Point2f(x2, y2), CV_RGB(0, 250, 0));

        imshow("w", frame);
        waitKey(20); // waits to display frame
    }
    return 0;
}