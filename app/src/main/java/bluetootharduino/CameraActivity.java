package bluetootharduino;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.Toast;

import com.example.bluetootharduino.R;

public class CameraActivity extends Activity implements OnGestureListener {

	private static final String TAG = "CameraActivity";
	
	GestureDetector detector;
	
	
	@SuppressWarnings("deprecation")
	@Override
    protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.camera_activity);
        

        detector = new GestureDetector(this);
       
        Toast.makeText(getApplicationContext(), "Swipe to Bluetooth", Toast.LENGTH_SHORT).show();
        
    }

	
	@Override
	public void onResume()
	{
	    super.onResume();
	    //OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
	}
	
	
	/** activity implements the camera view */
	
	 public void onPause() {
	     super.onPause();
	     //if (mOpenCvCameraView != null)
	     //    mOpenCvCameraView.disableView();
	 }

	 public void onDestroy() {
	     super.onDestroy();
	     //if (mOpenCvCameraView != null)
	     //    mOpenCvCameraView.disableView();
	 }
/*
	@Override
	public void onCameraViewStarted(int width, int height) {}

	@Override
	public void onCameraViewStopped() {}

	@Override
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		return inputFrame.rgba();
	}
*/

/** Uses swipe to change to the bluetooth activity*/
	
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
			/** Changes to Bluetooth screen */  	
       	    Intent changeToBluetooth= new Intent(getApplicationContext(), BluetoothActivity.class);
			startActivity(changeToBluetooth);
			return true;
		}
		
		return false;
	}
	
	public boolean onTouchEvent(MotionEvent ev) {
		return detector.onTouchEvent(ev);	
	} 

}
