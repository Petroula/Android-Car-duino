#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


extern void processImage(cv::Mat mat, int widthStep);

int main()
{
    string filename = "testdrive.mp4";
    VideoCapture capture(filename);
    Mat frame;

    if (!capture.isOpened())
    throw "Error when opening test4.avi";

    namedWindow("w", 1);
    for (;;)
    {
        capture >> frame;
        if (frame.empty())
        break;

        processImage(frame, frame.step);

        imshow("w", frame);
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
    return 0;
}