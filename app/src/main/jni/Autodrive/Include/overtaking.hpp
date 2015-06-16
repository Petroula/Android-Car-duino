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
        int oomphTurnLeft = 0;
        int turnRight = 0;
        int turnRightCalibration = 0;
        int oomphTurnRight = 0;
        bool turnLeftCalibrationFinished = false;
        int obstacleDistance = 70;
        bool obstacleMet = false;
        bool obstaclePassed = false;
        bool lineLeftFound = false;
        bool lineRightFound = false;
        bool stop = false;

        command run(command lastCommand, Mat* mat) {
            usFront = SensorData::ultrasound.front;
            usFrontRight = SensorData::ultrasound.frontright;
            irFrontRight = SensorData::infrared.frontright;
            irRearRight = SensorData::infrared.rearright;
            distanceTravelled = SensorData::encoderDistance();
            lineLeftFound = SensorData::lineLeftFound;
            lineRightFound = SensorData::lineRightFound;

            lastCommand.setSpeed(0.32);
            if (stop) lastCommand.setSpeed(0);

            if (usFront > 0 && usFront < obstacleDistance) {
                if (! overtaking) {
                    overtaking = true;
                    if (! turnLeft) turnLeft = distanceTravelled;
                    SensorData::lineLeftFound = false;
                    SensorData::lineRightFound = false;
                }
            }

            if (overtaking) { // obstacle spotted, start turning left
                if (turnLeft) {
                    lastCommand.setAngle(-1);

                    if (lineRightFound) {
                        turnLeft = 0;

                        if (! turnLeftCalibration) turnLeftCalibration = distanceTravelled;
                    }

                    if (lineLeftFound) {
                        turnLeft = 0;

                        if (! oomphTurnLeft) oomphTurnLeft = distanceTravelled;
                    }
                }

                if (oomphTurnLeft) {
                    if (distanceTravelled - oomphTurnLeft > 10) {
                        oomphTurnLeft = 0;
                        
                        if (! turnLeftCalibration) turnLeftCalibration = distanceTravelled;
                    }
                }

                if (turnLeftCalibration) {
                    lastCommand.setAngle(1);

                    if (distanceTravelled - turnLeftCalibration > 15) { // turn a bit to the right for 10cm to calibrate for easier lane following
                        turnLeftCalibration = 0;
                        turnLeftCalibrationFinished = true;
                        SensorData::lineLeftFound = false;
                        SensorData::lineRightFound = false;
                    }
                }

                if (turnLeftCalibrationFinished) {
                    if (irRearRight > 1 && irRearRight < 21) { // if irRearRight sees something it met the obstacle
                        if (! obstacleMet) {
                                turnLeftCalibrationFinished = false;
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
                        stop = true;

                        lastCommand.setAngle(1);

                        if (lineLeftFound) {
                            turnRight = 0;
                            if (! turnRightCalibration) turnRightCalibration = distanceTravelled;
                        }

                        if (lineRightFound){
                            turnRight = 0;
                            
                            if (! oomphTurnRight) oomphTurnRight = distanceTravelled;
                        }
                    }

                    if (oomphTurnRight) {
                        if (distanceTravelled - oomphTurnRight > 10) {
                            oomphTurnRight = 0;
                            stop = true;
                            if (! turnRightCalibration) turnRightCalibration = distanceTravelled;
                        }
                    }

                    if (turnRightCalibration) {
                        lastCommand.setAngle(-1);

                        if (distanceTravelled - turnRightCalibration > 20) { // turn a bit to the left for 10cm to calibrate for easier lane following
                            turnRightCalibration = 0;
                            obstacleMet = false;
                            obstaclePassed = false;
                            overtaking = false;
                            SensorData::lineLeftFound = false;
                            SensorData::lineRightFound = false;
                        }
                    }
                }
            }

            if (debugMode) {
                if (overtaking) {
                    cv::putText(*mat, "overtaking", POINT(50.f, mat->size().height / 6.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);

                    if (lineLeftFound) {
                        cv::putText(*mat, "line LEFT found", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                    }

                    if (lineRightFound) {
                        cv::putText(*mat, "line RIGHT found", POINT(50.f, mat->size().height / 2.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                    }
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

                if (stop) {
                    cv::putText(*mat, "stopped", POINT(50.f, mat->size().height / 6.f), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 2);
                }
            }

            return lastCommand;
        }
    }
}