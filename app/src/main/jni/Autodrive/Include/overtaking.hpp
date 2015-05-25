#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        float overtaking = 0;
        bool obstacleMet = false;
        float finaliseOvertaking = 0;

        command run(command lastCommand, Mat* mat) {
            int usFront = SensorData::ultrasound.front;
            int irFrontRight = SensorData::infrared.frontright;
            int irRearRight = SensorData::infrared.rearright;
            int objectDistance = 100;

            if (usFront > 0 && usFront < objectDistance) {
                if (overtaking < 1) overtaking = SensorData::encoderDistance();
            }

            if (overtaking > 0) {
                lastCommand.setSpeed(slowSpeed);

                if (! obstacleMet) {
                        cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                if ((SensorData::encoderDistance() - overtaking < 50)) {
                    if (! Status::isLeftLane) lastCommand.setAngle(-1); else lastCommand.setAngle(1);
                }

                if (irRearRight > 0 && irRearRight < 10) if (! obstacleMet) obstacleMet = true;

                if (obstacleMet) {
                    cv::putText(*mat, "obstacle met", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                    if (irFrontRight < 0 || irFrontRight > 10) {
                        overtaking = -1;
                        obstacleMet = false;

                        if (finaliseOvertaking < 0) finaliseOvertaking = SensorData::encoderDistance();
                    }
                }
            }

            if (finaliseOvertaking > 0) {
                lastCommand.setSpeed(slowSpeed);

                cv::putText(*mat, "back to lane", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                if (SensorData::encoderDistance() - finaliseOvertaking < 60) {
                    if (! Status::isLeftLane) lastCommand.setAngle(1); else lastCommand.setAngle(-1);
                } else {
                    finaliseOvertaking = -1;
                }
            }

            if (usFront > 0 && usFront < 15) lastCommand.setSpeed(0);

            return lastCommand;
        }
    }
}