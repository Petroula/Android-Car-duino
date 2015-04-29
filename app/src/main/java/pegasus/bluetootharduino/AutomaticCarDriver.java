package pegasus.bluetootharduino;

import org.opencv.core.Mat;

public class AutomaticCarDriver{

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("opencv_java");
        System.loadLibrary("autodrive");
    }

    public Mat processImage(Mat image) {
        Autodrive.setImage(image.getNativeObjAddr());
        Autodrive.drive();
        return image;
    }
}
