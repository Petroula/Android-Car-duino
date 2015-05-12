package pegasus.bluetootharduino;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.widget.SeekBar;
import android.widget.TextView;

public class AdvSettingsActivity extends Activity implements SeekBar.OnSeekBarChangeListener, GestureDetector.OnGestureListener {

    SeekBar kp, ki, kd, smoothening, fragment, leftIteration, rightIteration, angle;
    GestureDetector detector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.advanced_settings);

        detector = new GestureDetector(this);

        //SEEK BARS
        kp = (SeekBar)findViewById(R.id.kp);
        kp.setMax(50);
        kp.setOnSeekBarChangeListener(this);

        ki = (SeekBar)findViewById(R.id.ki);
        ki.setMax(50);
        ki.setOnSeekBarChangeListener(this);

        kd = (SeekBar)findViewById(R.id.kd);
        kd.setMax(50);
        kd.setOnSeekBarChangeListener(this);

        smoothening = (SeekBar)findViewById(R.id.smoothening);
        smoothening.setMax(8); // values 0-8
        smoothening.setOnSeekBarChangeListener(this);

        fragment = (SeekBar)findViewById(R.id.fragment);
        fragment.setMax(60); // values 15-60
        fragment.setOnSeekBarChangeListener(this);

        leftIteration = (SeekBar)findViewById(R.id.leftIteration);
        leftIteration.setMax(15); // values 1-15
        leftIteration.setOnSeekBarChangeListener(this);

        rightIteration = (SeekBar)findViewById(R.id.rightIteration);
        rightIteration.setMax(15); // values 1-15
        rightIteration.setOnSeekBarChangeListener(this);

        angle = (SeekBar)findViewById(R.id.angle);
        angle.setMax(8); // values 0.4-1.4
        angle.setOnSeekBarChangeListener(this);
    }

    /* SEEK BAR */
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId()) {
            case R.id.kp:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress1)).setText("kp value set to " + progressValue);
                    Autodrive.setPIDkp(progressValue);
                }
                break;
            case R.id.ki:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress2)).setText("ki value set to " + progressValue);
                    Autodrive.setPIDki(progressValue);
                }
                break;
            case R.id.kd:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress3)).setText("kd value set to " + progressValue);
                    Autodrive.setPIDkd(progressValue);
                }
                break;
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {}



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