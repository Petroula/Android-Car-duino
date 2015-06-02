#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "imageprocessor/imageprocessor.hpp"

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        bool overtaking = false;
        bool obstacleMet = false;
        bool finaliseOvertaking = false;
        bool stop = false;
        bool goToLeftLane = false;
        bool goToRightLane = false;
        bool oomph = false;
        bool dashed = false;

//        sensor usFront, irFrontRight, irRearRight;

        command run(command lastCommand, Mat* mat) {
//            lastCommand.setSpeed(0);

//            usFront.set(SensorData::ultrasound.front);
//            irFrontRight.set(SensorData::infrared.frontright);
//            irRearRight.set(SensorData::infrared.rearright);

//            char usFrontText[25], irFrontRightText[25], irRearRightText[25];
//            sprintf(usFrontText, "usFront: %d",  usFront.value);
//            sprintf(irFrontRightText, "irFrontRight: %d",  irFrontRight.value);
//            sprintf(irRearRightText, "irRearRight: %d", irRearRight.value);
//            cv::putText(*mat, usFrontText, POINT(50.f, mat->size().height / 6.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            cv::putText(*mat, irFrontRightText, POINT(50.f, mat->size().height / 4.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            cv::putText(*mat, irRearRightText, POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

//            usFront.previous = usFront.value;
//            irFrontRight.previous = irFrontRight.value;
//            irRearRight.previous = irRearRight.value;

//            return lastCommand;


            int usFront = SensorData::ultrasound.front;
            int irFrontRight = SensorData::infrared.frontright;
            int irRearRight = SensorData::infrared.rearright;
            int objectDistance = 70;

            if (usFront > 0 && usFront < objectDistance) {
                if (! overtaking) overtaking = true;
                if (! goToLeftLane) goToLeftLane = true;
            }

            if (overtaking && ! finaliseOvertaking) {
                if (! obstacleMet) {
                    cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                if (goToLeftLane) lastCommand.setAngle(-1);

                if (imageProcessor::dashedLineGaps() > 1) {
                    goToLeftLane = false;
                    dashed = true;
//                    oomph = SensorData::encoderDistance();
                }

                if (dashed) cv::putText(*mat, "dashed", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

//                if (SensorData::encoderDistance() - oomph < 30) {
//                    ran = true;
//                    cv::putText(*mat, "dashed", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//                    lastCommand.setAngle(0);
//                }
//
//                if (ran) cv::putText(*mat, "runs", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);



//                if (irRearRight > 5 && irRearRight < 10) if (! obstacleMet) obstacleMet = true;
//
//                if (obstacleMet) {
//                    cv::putText(*mat, "obstacle met", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//
//                    if (irFrontRight > 15 || ! irFrontRight) {
//                        overtaking = 0;
//                        obstacleMet = false;
//
//                        if (! finaliseOvertaking) finaliseOvertaking = SensorData::encoderDistance();
//                    }
//                }
            }

//            if (finaliseOvertaking) {
////                lastCommand.setSpeed(slowSpeed);
//
//                cv::putText(*mat, "back to lane", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//
//                if (! imageProcessor::isRightLane()) {
//                    lastCommand.setAngle(1);
//                } else if (imageProcessor::isLeftLane()) {
//                    finaliseOvertaking = 0;
//                }
//            }

//            if (stop) {
//                lastCommand.setSpeed(0);
//                cv::putText(*mat, "stop", POINT(50.f, mat->size().height / 1.5f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            }
//
//            if (usFront > 0 && usFront < 15) lastCommand.setSpeed(0);
//
//            if (imageProcessor::isLeftLane()) {
//                cv::putText(*mat, "left lane", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            } else if (imageProcessor::isRightLane()) {
//                cv::putText(*mat, "right lane", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            } else {
//                cv::putText(*mat, "not found", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
//            }

            lastCommand.setSpeed(0.25);
            return lastCommand;
        }
    }
}