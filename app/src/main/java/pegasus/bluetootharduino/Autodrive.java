package pegasus.bluetootharduino;

import org.opencv.core.Mat;

/**
 * Created by David on 2015-04-07.
 */
public class Autodrive
{
    public static native void drive();
    public static native void setImage(long matAddrRgba);
    public static native void setUltrasound(int sensor, int value);
    public static native int getUltrasoundSensor(int sensor);
    public static native void setInfrared(int sensor, int value);
    public static native int getInfraredSensor(int sensor);
    public static native void setEncoderPulses(int value);
    public static native double getEncoderDistance();

    public static native boolean speedChanged();
    public static native boolean angleChanged();
    public static native int getTargetSpeed();
    public static native int getTargetAngle();
}
