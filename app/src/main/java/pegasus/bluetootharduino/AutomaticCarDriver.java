package pegasus.bluetootharduino;

import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

public class AutomaticCarDriver{

    AutomaticCarDriver(){
        Autodrive.reset();
    }

    public Mat processImage(Mat image) {
        Mat resized = new Mat();
        Size prevSize = image.size();
        Size size = new Size(240,135);
        Imgproc.resize(image, resized, size);
        Autodrive.setImage(resized.getNativeObjAddr());
        Autodrive.drive();
        if(Settings.DisplayDebugInformation)
            Imgproc.resize(resized,image,prevSize);
        return image;
    }
}
