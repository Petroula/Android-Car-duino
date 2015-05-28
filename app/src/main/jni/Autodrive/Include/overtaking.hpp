#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        struct sensor {
            int previous = 0;
            int value = 0;
            int skipZero = true;
            void set(int val) {
                value = val;
                filter();
            };
            void filter() {
                int initValue = value;
                if (skipZero) if (! value && previous) value = previous;
                if (! initValue) skipZero = false;
            }
        };

        int overtaking = 0;
        bool obstacleMet = false;
        int finaliseOvertaking = 0;

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
            int objectDistance = 50;

            if (usFront > 0 && usFront < objectDistance) {
                if (! overtaking) overtaking = SensorData::encoderDistance();
            }

            if (overtaking && ! finaliseOvertaking) {
//                lastCommand.setSpeed(slowSpeed);

                if (! obstacleMet) {
                        cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                if ((SensorData::encoderDistance() - overtaking < 60)) {
                    lastCommand.setAngle(-1);
                }

                if (irRearRight > 5 && irRearRight < 10) if (! obstacleMet) obstacleMet = true;

                if (obstacleMet) {
                    cv::putText(*mat, "obstacle met", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                    if (irFrontRight > 15 || ! irFrontRight) {
                        overtaking = 0;
                        obstacleMet = false;

                        if (! finaliseOvertaking) finaliseOvertaking = SensorData::encoderDistance();
                    }
                }
            }

            if (finaliseOvertaking) {
//                lastCommand.setSpeed(slowSpeed);

                cv::putText(*mat, "back to lane", POINT(50.f, mat->size().height / 3.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                if (SensorData::encoderDistance() - finaliseOvertaking < 40) {
                    lastCommand.setAngle(1);
                } else {
                    finaliseOvertaking = 0;
                }
            }

            if (usFront > 0 && usFront < 15) lastCommand.setSpeed(0);

            lastCommand.setSpeed(normalSpeed);

            return lastCommand;
        }
    }
}