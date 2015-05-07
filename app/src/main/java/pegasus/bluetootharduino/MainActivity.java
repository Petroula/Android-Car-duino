package pegasus.bluetootharduino;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener, CompoundButton.OnCheckedChangeListener, SeekBar.OnSeekBarChangeListener {


    SeekBar kp, ki, kd;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        /* BUTTONS */
        ((Button)findViewById(R.id.auto)).setOnClickListener(this);
        ((Button)findViewById(R.id.parking)).setOnClickListener(this);
        ((Button)findViewById(R.id.manual)).setOnClickListener(this);

        //DISPLAY DEBUG INFORMATION SWITCH
        ((Switch)findViewById(R.id.DisplayDebugSwitch)).setOnCheckedChangeListener(this);
        //USE LIGHT NORMALIZATION SWITCH
        ((Switch)findViewById(R.id.LightNormalizationSwitch)).setOnCheckedChangeListener(this);

        //PID SEEK BARS
        ((SeekBar)findViewById(R.id.InnerIterationLength)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.outerIterationLength)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.smoothening)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.ForwardWhenLostNFrames)).setOnSeekBarChangeListener(this);

        //PID DESCRIPTIONS
        ((TextView)findViewById(R.id.progress1)).setText("InnerIterationLength value set to 10");
        ((TextView)findViewById(R.id.outerIterationLengthText)).setText("outerIterationLength value set to 6");
        ((TextView)findViewById(R.id.lostThresholdText)).setText("lostThreshold value set to 0");
        ((TextView)findViewById(R.id.smootheningText)).setText("smoothing value set to 2");

    }

    /* BUTTONS */
    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.manual:
                break;
            case R.id.parking:
            case R.id.auto:
                Intent changeToCamera= new Intent(getApplicationContext(),
                CameraActivity.class);
                startActivity(changeToCamera);
        }
    }

    /* SWITCHES */
    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        switch (buttonView.getId()) {
            case R.id.LightNormalizationSwitch:
                Autodrive.setSettingLightNormalization(isChecked);
                break;
            case R.id.DisplayDebugSwitch:
                Settings.DisplayDebugInformation = isChecked;
                break;
        }
    }

    /* SEEK BAR */
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId()) {
            case R.id.outerIterationLength:
                if(fromUser) {
                    ((TextView)findViewById(R.id.outerIterationLengthText)).setText("outerIterationLength set to " + progress);
                    Autodrive.setSettingRightIterationLength(progress);
                }
                break;

            case R.id.InnerIterationLength:
                if(fromUser) {
                    ((TextView)findViewById(R.id.progress1)).setText("InnerIterationLength set to " + progress);
                    Autodrive.setSettingLeftIterationLength(progress);
                }
                break;
            case R.id.ForwardWhenLostNFrames:
                    if(progress > 0) {
                        ((TextView) findViewById(R.id.lostThresholdText)).setText("will reset angle after " + progress + " frames");
                    }else {
                        ((TextView) findViewById(R.id.lostThresholdText)).setText("will never reset angle");
                    }
                    Autodrive.setForwardWhenLost(progress);
                break;
            case R.id.smoothening:
                if(fromUser) {
                    ((TextView)findViewById(R.id.smootheningText)).setText("smoothening value set to " + progress);
                    Autodrive.setSettingSmoothening(progress);
                }
                break;
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {}
}
