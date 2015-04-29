package pegasus.bluetootharduino;

import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

public class AutomaticCarDriver{

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("opencv_java");
        System.loadLibrary("autodrive");
    }

    public Mat processImage(Mat image) {
        Mat resized = new Mat();
        Size prevSize = image.size();
        Size size = new Size(320,180);
        Imgproc.resize(image,resized,size);
        Autodrive.setImage(resized.getNativeObjAddr());
        Autodrive.drive();
        Imgproc.resize(resized,image,prevSize);
        return image;
    }
}
