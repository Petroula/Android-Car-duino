package pegasus.bluetootharduino;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import java.io.IOException;
import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


public class CameraActivity extends Activity implements CvCameraViewListener2, OnGestureListener {

    private CameraBridgeViewBase mOpenCvCameraView;
    private static final String TAG = "CameraActivity";
    static TextView debugConsole;

    GestureDetector detector;
    AutomaticCarDriver driver = new AutomaticCarDriver();

    Bluetooth bt = new Bluetooth();

    @SuppressWarnings("deprecation")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.camera_activity);
        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.CameraView);
        //mOpenCvCameraView.setMaxFrameSize(240,135);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);

        detector = new GestureDetector(this);

        debugConsole = (TextView) findViewById(R.id.debugConsole);

        bt.checkBT();

        //if device does not support bluetooth (not really needed)
        if(bt.adapter == null) {
            Intent enable = new Intent(String.valueOf(BluetoothAdapter.ERROR));
            startActivityForResult(enable,  0);
        }

        if(!bt.btEnabled) {
            Intent enable = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enable,  1);
        }
        try {
            bt.runBT();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static public void updateDebuggingConsole() {
        debugConsole.setText("");
        debugConsole.append("ultrasonicFront: " + SensorData.ultrasonicFront + "\n");
        debugConsole.append("ultrasonicFrontRight: " + SensorData.ultrasonicFrontRight + "\n");
        debugConsole.append("ultrasonicRear: " + SensorData.ultrasonicRear + "\n");
        debugConsole.append("infraredSideFront: " + SensorData.infraredSideFront + "\n");
        debugConsole.append("infraredSideRear: " + SensorData.infraredSideRear + "\n");
        debugConsole.append("infraredRear: " + SensorData.infraredRear + "\n");
        debugConsole.append("gyroHeading: " + SensorData.gyroHeading + "\n");
        debugConsole.append("razorHeading: " + SensorData.razorHeading + "\n");
        debugConsole.append("encoderPulses: " + SensorData.encoderPulses + "\n");
        debugConsole.append("Obstacle Found: " + ((SensorData.foundObstacle()) ? "true" : "false") + "\n");
        debugConsole.append("irFrontAutodrive: " + String.valueOf(SensorData.irFrontAutodrive()) + "\n");
        debugConsole.append("usFrontAutodrive: " + String.valueOf(SensorData.usFrontAutodrive()) + "\n");
    }

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };


    @Override
    public void onResume()
    {
        driver = new AutomaticCarDriver();
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
    }


    /** activity implements the camera view */

    public void onPause() {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onCameraViewStarted(int width, int height) {}

    @Override
    public void onCameraViewStopped() {}

    @Override
    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        return driver.processImage(inputFrame.rgba());
    }



    /** Uses swipe to change to the main activity*/

    @Override
    public boolean onDown(MotionEvent e) {
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {}

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {}

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,	float velocityY) {

        if(e1.getX()<e2.getX()) {
            // disconnect safely
            if(bt.socket.isConnected()) {
                bt.disconnect();
            }
            /** Changes to Main screen */
            Intent changeToMain= new Intent(getApplicationContext(), MainActivity.class);
            startActivity(changeToMain);
            return true;
        }

        return false;
    }

    public boolean onTouchEvent(MotionEvent ev) {
        return detector.onTouchEvent(ev);
    }

}
