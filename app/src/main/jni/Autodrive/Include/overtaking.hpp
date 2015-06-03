#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "imageprocessor/imageprocessor.hpp"

using namespace cv;

namespace Autodrive {
    namespace Overtaking {
        bool debugMode = true;

        int usFront = 0;
        int usFrontRight = 0;
        int irFrontRight = 0;
        int irRearRight = 0;
        int distanceTravelled = 0;

        bool overtaking = false;
        int turnLeft = 0;
        int turnLeftCalibration = 0;
        int turnRight = 0;
        int turnRightCalibration = 0;
        bool turnRightCalibrationFinished = false;
        int obstacleDistance = 70;
        bool obstacleMet = false;
        bool obstaclePassed = false;

        command run(command lastCommand, Mat* mat) {
            usFront = SensorData::ultrasound.front;
            usFrontRight = SensorData::ultrasound.frontright;
            irFrontRight = SensorData::infrared.frontright;
            irRearRight = SensorData::infrared.rearright;
            distanceTravelled = SensorData::encoderDistance();

            if (usFront > 0 && usFront < obstacleDistance) {
                if (! overtaking) {
                    overtaking = true;
                    if (! turnLeft) turnLeft = distanceTravelled;
                }
            }

            if (overtaking) { // obstacle spotted, start turning left
                if (turnLeft) {
                    lastCommand.setAngle(-1);

                    if (distanceTravelled - turnLeft > 50) { // turn left for 50 cm
                        turnLeft = 0;

                        if (! turnLeftCalibration) turnLeftCalibration = distanceTravelled;
                    }
                }

                if (turnLeftCalibration) {
                    lastCommand.setAngle(1);

                    if (distanceTravelled - turnLeftCalibration > 10) { // turn a bit to the right for 10cm to calibrate for easier lane following
                        turnLeftCalibration = 0;
                        turnRightCalibrationFinished = true;
                    }
                }

                if (turnRightCalibrationFinished) {
                    if (irRearRight > 1 && irRearRight < 21) { // if irRearRight sees something it met the obstacle
                        if (! obstacleMet) {
                                turnRightCalibrationFinished = false;
                                obstacleMet = true;
                            }
                    }
                }

                if (obstacleMet) {
                    if (! usFrontRight || usFrontRight > 20) { // if usFrontRight doesn't see anything, it passed the obstacle
                        if (! obstaclePassed) {
                            if (! turnRight) turnRight = distanceTravelled; // so start turning right

                            obstaclePassed = true;
                        }

                    }

                    if (turnRight) {
                        lastCommand.setAngle(1);

                        if (distanceTravelled - turnRight > 50) { // turn right for 50cm
                            turnRight = 0;
                            if (! turnRightCalibration) turnRightCalibration = distanceTravelled;
                        }
                    }

                    if (turnRightCalibration) {
                        lastCommand.setAngle(-1);

                        if (distanceTravelled - turnRightCalibration > 10) { // turn a bit to the left for 10cm to calibrate for easier lane following
                            turnRightCalibration = 0;
                            obstacleMet = false;
                            obstaclePassed = false;
                            overtaking = false;
                        }
                    }
                }
            }

            if (debugMode) {
                if (overtaking) {
                    cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 6.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                char turningLeftText[50];
                sprintf(turningLeftText, "turning left: %d", turnLeft);
                if (turnLeft) {
                    cv::putText(*mat, turningLeftText, POINT(50.f, mat->size().height / 4.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                char turningRightText[50];
                sprintf(turningRightText, "turning right: %d", turnRight);
                if (turnRight) {
                    cv::putText(*mat, turningRightText, POINT(50.f, mat->size().height / 4.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }

                if (obstacleMet) {
                    cv::putText(*mat, "obstacle met", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }
            }

            lastCommand.setSpeed(normalSpeed);

            return lastCommand;
        }
    }
}