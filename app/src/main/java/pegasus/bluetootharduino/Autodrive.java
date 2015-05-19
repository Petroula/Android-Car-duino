package pegasus.bluetootharduino;

import org.opencv.core.Mat;

/**
 * Created by David on 2015-04-07.
 */
public class Autodrive
{
    static
    {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("opencv_java");
        System.loadLibrary("autodrive");
    }

    public static native void reset();
    public static native void drive();
    public static native void setParkingMode();
    
    public static native void setCarLength(int carLength);
    public static native int getCarLength();
    
/*----- DEBUGDATA -----*/
    public static native int gapLength();
    public static native int getManeuver();
    public static native int getManeuverState();
    
    public static native int angleTurned();
    
    public static String maneuver()
    {
        switch(getManeuver())
        {
            case 0:
                return "NO_MANEUVER";
            case 1:
                return "PARALLEL_STANDARD";
            case 2:
                return "PARALLEL_WIDE";
            case 3:
                return "PERPENDICULAR_STANDARD";
           default:
                return "ERR";
        }
    }
    
    public static String maneuverstate()
    {
        switch(getManeuverState())
        {
            case 0:
                return "NOT_MOVING";
            case 1:
                return "FORWARD";
            case 2:
                return "BACKWARD";
            case 3:
                return "FORWARD_RIGHT";
            case 4:
                return "BACKWARD_RIGHT";
            case 5:
                return "FORWARD_LEFT";
            case 6:
                return "BACKWARD_LEFT";
            case 7:
                return "DONE";
            default:
                return "ERR";
        }
    }

/*----- SENSORDATA -----*/
    // getters - for debuging purposes   
    public static native int usFront();
    public static native int usFrontRight();
    public static native int usRear();
    public static native int irFrontRight();
    public static native int irRearRight();
    public static native int irRear();
    public static native int gyroHeading();
    public static native int razorHeading();

    public static native void setImage(long matAddrRgba);
    public static native void setUltrasound(int sensor, int value);
    public static native void setInfrared(int sensor, int value);
    public static native void setEncoderPulses(long value);
    public static native void setGyroHeading(int value);
    public static native void setRazorHeading(int value);

/*----- RESULTING AUTODRIVE DATA -----*/
    public static native boolean speedChanged();
    public static native boolean angleChanged();
    private static native double getTargetSpeed();
    private static native double getTargetAngle();

    public static int getConvertedSpeed(){
        double targetSpeed = getTargetSpeed();
        if(targetSpeed <= 0.0)
            return (int)(targetSpeed * carConfiguration.maxSpeed * 2.0);
        else
            return (int)(targetSpeed * carConfiguration.maxSpeed);
    }

    public static int getConvertedAngle(){
        return (int)(getTargetAngle() * carConfiguration.maxAngle);
    }

/*---- SETTINGS -----*/
    public static native void setSettingLightNormalization(boolean on);
    public static native void setSettingUseLeftLine(boolean on);

    // N Frames to take the mean value from, value should be between 0 - 8
    public static native void setSettingSmoothening(int value);

    // Maximum vertical distance to the first pixel from carY, value should be between 15-60
    public static native void setSettingFirstFragmentMaxDist(int value);

    // How many pixels to iterate to the left, for each pixel, value should be between 1-15
    public static native void setSettingLeftIterationLength(int value);

    // How many pixels to iterate to the right, for each pixel, value should be between 1-15
    public static native void setSettingRightIterationLength(int value);

    // Every pixel in a line can not have an angle from the previous pixel that deviates more than this,
    // value should be between 0.4-1.4
    public static native void setSettingMaxAngleDiff(float value);

    public static native void setPIDkp(float value);

    public static native void setPIDki(float value);

    public static native void setPIDkd(float value);
}
