package pegasus.bluetootharduino;

import org.opencv.core.Mat;

public class AutomaticCarDriver{

    public AutomaticCarDriver(){
        System.loadLibrary("autodrive");
    }

    public Mat processImage(Mat image) {
        float diff = Natives.processImage(image.getNativeObjAddr());
        return image;
    }
}
