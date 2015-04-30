package pegasus.bluetootharduino;

/**
 * Created by david on 2015-04-29.
 */
import java.util.List;

import org.opencv.android.NativeCameraView;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;

import android.content.Context;
import android.util.AttributeSet;

public class NativeCamResView extends NativeCameraView {

    public NativeCamResView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void init(){
        mCamera.set(Highgui.CV_CAP_PROP_ANDROID_FLASH_MODE, Highgui.CV_CAP_ANDROID_FLASH_MODE_OFF);
        mCamera.set(Highgui.CV_CAP_PROP_ANDROID_FOCUS_MODE, Highgui.CV_CAP_ANDROID_FOCUS_MODE_FIXED);
    }

    public List<Size> getResolutionList() {
        return mCamera.getSupportedPreviewSizes();
    }

    public void setResolution(Size resolution) {
        disconnectCamera();
        connectCamera((int)resolution.width, (int)resolution.height);
    }

    public Size getResolution() {
        Size s = new Size(mCamera.get(Highgui.CV_CAP_PROP_FRAME_WIDTH),mCamera.get(Highgui.CV_CAP_PROP_FRAME_HEIGHT));
        return s;
    }
}