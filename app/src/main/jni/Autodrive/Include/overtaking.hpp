#include "sensordata.hpp"
#include "maneuver.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        int usFront = SensorData::ultrasound.front;
        int usFrontRight = SensorData::ultrasound.frontright;
        int irFrontRight = SensorData::infrared.frontright;
        int irRearRight = SensorData::infrared.rearright;
        int objectDistance = 50;
        float overtaking = 0;
        bool obstacleMet = false;


        command run(command lastCommand, Mat* mat) {
            if (usFront > 0 && usFront < objectDistance) {
                if (overtaking < 1) overtaking = SensorData::encoderDistance();;
            }

            if (overtaking > 0) {
                if (! obstacleMet) {
                    cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                lastCommand.setSpeed(slowSpeed);

                if ((SensorData::encoderDistance() - overtaking < 40)) {
                    lastCommand.setAngle(-1);
                }

                if (irRearRight > 0 && irRearRight < 10) if (! obstacleMet) obstacleMet = true;

                if (obstacleMet) {
                    cv::putText(*mat, "obstacle met", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                    if ((irFrontRight < 0 || irFrontRight > 20) && ! (usFrontRight > 0 && usFrontRight < objectDistance)) {
                        overtaking = -1;
                        obstacleMet = false;

                        if (finaliseOvertaking < 0) finaliseOvertaking = SensorData::encoderDistance();
                    }
                }

                if (finaliseOvertaking > 0) {
                    cv::putText(*mat, "back to lane", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                    if (SensorData::encoderDistance() - finaliseOvertaking < 75) {
                        lastCommand.setAngle(1);
                    } else {
                        finaliseOvertaking = -1;
                    }
                }
            }

             return lastCommand;
        }
    }
}