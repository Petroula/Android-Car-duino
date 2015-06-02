#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "imageprocessor/imageprocessor.hpp"

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        bool overtaking = false;
        bool finaliseOvertaking = false;
        bool turnLeft = false;
        bool turnRight = false;
        int oomph = 0;
        bool obstacleMet = false;

        command run(command lastCommand, Mat* mat) {
            int usFront = SensorData::ultrasound.front;
            int irFrontRight = SensorData::infrared.frontright;
            int irRearRight = SensorData::infrared.rearright;
            int objectDistance = 70;

            if (usFront > 0 && usFront < objectDistance) {
                if (! overtaking) overtaking = true;
                if (! turnLeft) turnLeft = true;
            }

            if (overtaking) {
                if (turnLeft) lastCommand.setAngle(-1);

                if (imageProcessor::dashedLineGaps() > 1) {
                    turnLeft = false;
                    oomph = SensorData::encoderDistance();
                }

                if (SensorData::encoderDistance() - oomph < 15) {
                    lastCommand.setAngle(0);
                }

                if (irRearRight > 5 && irRearRight < 10) if (! obstacleMet) obstacleMet = true;

                if (obstacleMet) {
                    if (! irFrontRight) {
                        obstacleMet = false;
                        oomph = 0;

                        if (! finaliseOvertaking) finaliseOvertaking = true;
                        if (! turnRight) turnRight = true;
                    }
                }

                if (finaliseOvertaking) {
                    if (turnRight) lastCommand.setAngle(1);

                    if (imageProcessor::dashedLineGaps() > 1) {
                        turnRight = false;
                        oomph = SensorData::encoderDistance();
                    }

                    if (SensorData::encoderDistance() - oomph < 15) {
                        lastCommand.setAngle(0);
                    }

                    if (SensorData::encoderDistance() - oomph > 15) {
                        turnRight = false;
                        finaliseOvertaking = false;
                        overtaking = false;
                        oomph = 0;
                    }
                }
            }

            if (overtaking) {
                cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 4.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            if (turnLeft) {
                cv::putText(*mat, "turning left", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            if (turnRight) {
                cv::putText(*mat, "turning right", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            if (oomph) {
                cv::putText(*mat, "oomph", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            if (obstacleMet) {
                cv::putText(*mat, "oomph", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            if (finaliseOvertaking) {
                cv::putText(*mat, "finalise overtaking", POINT(50.f, mat->size().height / 1.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
            }

            lastCommand.setSpeed(0.26);
            return lastCommand;
        }
    }
}