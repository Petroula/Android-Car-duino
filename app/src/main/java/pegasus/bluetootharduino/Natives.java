package pegasus.bluetootharduino;

import org.opencv.core.Mat;

/**
 * Created by David on 2015-04-07.
 */
public class Natives {
    public static native float processImage(long matAddrRgba);
}
