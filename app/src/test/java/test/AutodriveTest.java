package test;

import junit.framework.TestCase;

import org.opencv.core.CvType;
import org.opencv.core.Mat;

import pegasus.bluetootharduino.Autodrive;

public class AutodriveTest extends TestCase {

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("opencv_java");
        System.loadLibrary("autodrive");
    }

    public void testTick() throws Exception {
        Mat mat = new Mat(300,300, CvType.CV_64FC1);
        Autodrive.setImage(mat.getNativeObjAddr());
    }
}