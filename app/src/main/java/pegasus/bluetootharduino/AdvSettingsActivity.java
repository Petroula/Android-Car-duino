package pegasus.bluetootharduino;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.widget.SeekBar;
import android.widget.TextView;

public class AdvSettingsActivity extends Activity implements SeekBar.OnSeekBarChangeListener, GestureDetector.OnGestureListener {

    SeekBar kp, ki, kd, smoothening, fragment, leftIteration, rightIteration, angle;
    GestureDetector detector;
    SharedPreferences shared;
    float progressValue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.advanced_settings);

        shared = PreferenceManager.getDefaultSharedPreferences(this);

        detector = new GestureDetector(this);

        //SEEK BARS
        kp = (SeekBar)findViewById(R.id.kp);
        kp.setMax(50);
        ((TextView)findViewById(R.id.progress1)).setText("kp value set to " + shared.getFloat("kp", progressValue));
        kp.setProgress((int) (shared.getFloat("kp", progressValue) *10));
        kp.setOnSeekBarChangeListener(this);

        ki = (SeekBar)findViewById(R.id.ki);
        ki.setMax(50);
        ((TextView)findViewById(R.id.progress2)).setText("ki value set to " + shared.getFloat("ki", progressValue));
        ki.setProgress((int) (shared.getFloat("ki", progressValue) *10));
        ki.setOnSeekBarChangeListener(this);

        kd = (SeekBar)findViewById(R.id.kd);
        kd.setMax(50);
        ((TextView)findViewById(R.id.progress3)).setText("kd value set to " + shared.getFloat("kd", progressValue));
        kd.setProgress((int) (shared.getFloat("kd", progressValue) *10));
        kd.setOnSeekBarChangeListener(this);

        smoothening = (SeekBar)findViewById(R.id.smoothening);
        smoothening.setMax(8); // values 0-8
        ((TextView)findViewById(R.id.progress4)).setText("Smoothening value set to " + shared.getFloat("smoothening", progressValue));
        smoothening.setProgress((int) (shared.getFloat("smoothening", progressValue)));
        smoothening.setOnSeekBarChangeListener(this);

        fragment = (SeekBar)findViewById(R.id.fragment);
        fragment.setMax(60); // values 15-60
        ((TextView)findViewById(R.id.progress5)).setText("Fragment distance value set to " + shared.getFloat("fragment", progressValue));
        fragment.setProgress((int) (shared.getFloat("fragment", progressValue)));
        fragment.setOnSeekBarChangeListener(this);

        leftIteration = (SeekBar)findViewById(R.id.leftIteration);
        leftIteration.setMax(15); // values 1-15
        ((TextView)findViewById(R.id.progress6)).setText("Left Iteration value set to " + shared.getFloat("leftIteration", progressValue));
        leftIteration.setProgress((int) (shared.getFloat("leftIteration", progressValue)));
        leftIteration.setOnSeekBarChangeListener(this);

        rightIteration = (SeekBar)findViewById(R.id.rightIteration);
        rightIteration.setMax(15); // values 1-15
        ((TextView)findViewById(R.id.progress7)).setText("Right Iteration value set to " + shared.getFloat("rightIteration", progressValue));
        rightIteration.setProgress((int) (shared.getFloat("rightIteration", progressValue)));
        rightIteration.setOnSeekBarChangeListener(this);

        angle = (SeekBar)findViewById(R.id.angle);
        angle.setMax(14); // values 0.4-1.4
        ((TextView)findViewById(R.id.progress8)).setText("Max angle value set to " + shared.getFloat("angle", progressValue));
        angle.setProgress((int) (shared.getFloat("angle", progressValue) *10));
        angle.setOnSeekBarChangeListener(this);
    }

    /* SEEK BAR */
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        switch (seekBar.getId()) {
            case R.id.kp:
                if(fromUser) {
                    progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress1)).setText("kp value set to " + progressValue);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("kp", progressValue);
                    sharedEditor.apply();
                    Autodrive.setPIDkp(progressValue);
                }
                break;
            case R.id.ki:
                if(fromUser) {
                    progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress2)).setText("ki value set to " + progressValue);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("ki", progressValue);
                    sharedEditor.apply();
                    Autodrive.setPIDki(progressValue);
                }
                break;
            case R.id.kd:
                if(fromUser) {
                    progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress3)).setText("kd value set to " + progressValue);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("kd", progressValue);
                    sharedEditor.apply();
                    Autodrive.setPIDkd(progressValue);
                }
                break;
            case R.id.smoothening:
                if(fromUser) {
                    progressValue = progress;
                    ((TextView)findViewById(R.id.progress4)).setText("Smoothening value set to " + (float)progress);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("smoothening", progressValue);
                    sharedEditor.apply();
                    Autodrive.setSettingSmoothening(progress);
                }
                break;
            case R.id.fragment:
                if(fromUser) {
                    if(progress < 15) {
                        progress = 15;
                        fragment.setProgress(progress);
                    }
                    progressValue = progress;
                    ((TextView)findViewById(R.id.progress5)).setText("Fragment distance value set to " + (float)progress);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("fragment", progressValue);
                    sharedEditor.apply();
                    Autodrive.setSettingFirstFragmentMaxDist(progress);
                }
                break;
            case R.id.leftIteration:
                if(fromUser) {
                    if(progress < 1) {
                        progress = 1;
                        leftIteration.setProgress(progress);
                    }
                    progressValue = progress;
                    ((TextView)findViewById(R.id.progress6)).setText("Left Iteration value set to " + (float)progress);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("leftIteration", progressValue);
                    sharedEditor.apply();
                    Autodrive.setSettingLeftIterationLength(progress);
                }
                break;
            case R.id.rightIteration:
                if(fromUser) {
                    if(progress < 1) {
                        progress = 1;
                        rightIteration.setProgress(progress);
                    }
                    progressValue = progress;
                    ((TextView)findViewById(R.id.progress7)).setText("Right Iteration value set to " + (float)progress);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("rightIteration", progressValue);
                    sharedEditor.apply();
                    Autodrive.setSettingRightIterationLength(progress);
                }
                break;
            case R.id.angle:
                if(fromUser) {
                    progressValue = (float) (progress / 10.0);
                    if(progressValue < 0.4) {
                        progressValue = (float) 0.4;
                        angle.setProgress((int) progressValue);
                    } else if(progressValue > 1.4) {
                        progressValue = (float) 1.4;
                        angle.setProgress((int) progressValue);
                    }
                    ((TextView)findViewById(R.id.progress8)).setText("Max angle value set to " + progressValue);
                    SharedPreferences.Editor sharedEditor = shared.edit();
                    sharedEditor.putFloat("angle", progressValue);
                    sharedEditor.apply();
                    Autodrive.setSettingMaxAngleDiff(progressValue);
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

    /** Changes the behaviour of the back button */
    public void onBackPressed() {
        Intent changeToMain= new Intent(getApplicationContext(), MainActivity.class);
        startActivity(changeToMain);
    }
}