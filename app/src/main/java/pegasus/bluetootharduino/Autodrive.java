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

/*----- SENSORDATA -----*/
    public static native void setImage(long matAddrRgba);
    public static native void setUltrasound(int sensor, int value);
    public static native int getUltrasoundSensor(int sensor);
    public static native void setInfrared(int sensor, int value);
    public static native int getInfraredSensor(int sensor);
    public static native void setEncoderPulses(int value);
    public static native double getEncoderDistance();

/*----- RESULTING AUTODRIVE DATA -----*/
    public static native boolean speedChanged();
    public static native boolean angleChanged();
    public static native int getTargetSpeed();
    public static native int getTargetAngle();

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
